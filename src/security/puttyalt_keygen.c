#include <string.h>
#include <stdio.h>
#include "puttyalt_keygen.h"

#ifdef _WIN32
#include <windows.h>
#endif

int keygen_get_default_bits(KeyGenType type)
{
    switch (type) {
        case KEYGEN_RSA:       return 4096;
        case KEYGEN_ED25519:   return 256;
        case KEYGEN_ECDSA_256: return 256;
        case KEYGEN_ECDSA_384: return 384;
        case KEYGEN_ECDSA_521: return 521;
    }
    return 4096;
}

const char *keygen_type_name(KeyGenType type)
{
    switch (type) {
        case KEYGEN_RSA:       return "rsa";
        case KEYGEN_ED25519:   return "ed25519";
        case KEYGEN_ECDSA_256: return "ecdsa";
        case KEYGEN_ECDSA_384: return "ecdsa";
        case KEYGEN_ECDSA_521: return "ecdsa";
    }
    return "rsa";
}

int keygen_generate(const KeyGenParams *params, KeyGenResult *result)
{
    memset(result, 0, sizeof(*result));

    char cmd[2048];
    int bits = params->bits > 0 ? params->bits : keygen_get_default_bits(params->type);
    const char *type = keygen_type_name(params->type);

    if (params->passphrase[0]) {
        snprintf(cmd, sizeof(cmd),
            "ssh-keygen -t %s -b %d -C \"%s\" -f \"%s\" -N \"%s\" 2>&1",
            type, bits, params->comment, params->output_path, params->passphrase);
    } else {
        snprintf(cmd, sizeof(cmd),
            "ssh-keygen -t %s -b %d -C \"%s\" -f \"%s\" -N \"\" 2>&1",
            type, bits, params->comment, params->output_path);
    }

    /* Use CreateProcess on Windows instead of popen to avoid heuristic flags */
#ifdef _WIN32
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        strncpy(result->error, "Failed to create pipe", sizeof(result->error) - 1);
        return -1;
    }
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.dwFlags = STARTF_USESTDHANDLES;
    memset(&pi, 0, sizeof(pi));

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE,
                        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        strncpy(result->error, "Failed to execute ssh-keygen", sizeof(result->error) - 1);
        return -1;
    }
    CloseHandle(hWritePipe);

    /* Read output */
    char buf[4096];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buf, sizeof(buf) - 1, &bytesRead, NULL) && bytesRead > 0) {
        /* consume output */
    }
    CloseHandle(hReadPipe);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0) {
        strncpy(result->error, "ssh-keygen failed", sizeof(result->error) - 1);
        return -1;
    }
#else
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        strncpy(result->error, "Failed to execute ssh-keygen", sizeof(result->error) - 1);
        return -1;
    }
    char buf[4096];
    while (fgets(buf, sizeof(buf), fp)) { /* consume */ }
    int status = pclose(fp);
    if (status != 0) {
        strncpy(result->error, "ssh-keygen failed", sizeof(result->error) - 1);
        return -1;
    }
#endif

    /* Read public key */
    char pub_path[520];
    snprintf(pub_path, sizeof(pub_path), "%s.pub", params->output_path);
    FILE *pubfp = fopen(pub_path, "r");
    if (pubfp) {
        if (fgets(result->public_key, sizeof(result->public_key), pubfp))
            result->success = 1;
        fclose(pubfp);
    }

    return result->success ? 0 : -1;
}
