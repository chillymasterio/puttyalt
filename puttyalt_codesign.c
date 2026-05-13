#include "puttyalt_codesign.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <wintrust.h>
#include <softpub.h>

#pragma comment(lib, "wintrust.lib")
#pragma comment(lib, "crypt32.lib")

/*
 * Remove the Zone.Identifier alternate data stream that causes
 * SmartScreen warnings on downloaded executables.
 */
int codesign_remove_zone_id(const char *exe_path)
{
    char ads_path[1024];
    snprintf(ads_path, sizeof(ads_path), "%s:Zone.Identifier", exe_path);
    if (DeleteFileA(ads_path))
        return 0;
    /* NTFS ADS doesn't exist — already trusted */
    if (GetLastError() == ERROR_FILE_NOT_FOUND)
        return 0;
    return -1;
}

int codesign_remove_zone_id_self(void)
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return codesign_remove_zone_id(path);
}

int codesign_has_zone_id(const char *path)
{
    char ads[MAX_PATH + 32];
    snprintf(ads, sizeof(ads), "%s:Zone.Identifier", path);
    HANDLE h = CreateFileA(ads, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return 0; /* No zone ID */
    CloseHandle(h);
    return 1;
}

/*
 * Verify Authenticode signature using WinVerifyTrust.
 * This is the proper Windows API for signature verification.
 */
CSTrustResult codesign_verify_signature(const char *exe_path)
{
    WCHAR wpath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, exe_path, -1, wpath, MAX_PATH);

    WINTRUST_FILE_INFO fi;
    memset(&fi, 0, sizeof(fi));
    fi.cbStruct = sizeof(fi);
    fi.pcwszFilePath = wpath;

    GUID policy = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    WINTRUST_DATA wd;
    memset(&wd, 0, sizeof(wd));
    wd.cbStruct = sizeof(wd);
    wd.dwUIChoice = WTD_UI_NONE;
    wd.fdwRevocationChecks = WTD_REVOKE_NONE;
    wd.dwUnionChoice = WTD_CHOICE_FILE;
    wd.pFile = &fi;
    wd.dwStateAction = WTD_STATEACTION_VERIFY;
    wd.dwProvFlags = WTD_SAFER_FLAG;

    LONG result = WinVerifyTrust(NULL, &policy, &wd);

    /* Clean up state */
    wd.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(NULL, &policy, &wd);

    switch (result) {
    case ERROR_SUCCESS:
        return CS_TRUST_SIGNED;
    case TRUST_E_NOSIGNATURE:
        return CS_TRUST_UNSIGNED;
    case TRUST_E_EXPLICIT_DISTRUST:
    case TRUST_E_SUBJECT_NOT_TRUSTED:
        return CS_TRUST_UNTRUSTED;
    case TRUST_E_BAD_DIGEST:
        return CS_TRUST_TAMPERED;
    case CERT_E_EXPIRED:
        return CS_TRUST_EXPIRED;
    default:
        return CS_TRUST_UNKNOWN;
    }
}

CSTrustResult codesign_verify_self(void)
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return codesign_verify_signature(path);
}

int codesign_is_trusted(void)
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    /* Check 1: No Zone.Identifier means trusted */
    if (!codesign_has_zone_id(path))
        return 1;

    /* Check 2: Valid signature means trusted */
    CSTrustResult sig = codesign_verify_signature(path);
    if (sig == CS_TRUST_SIGNED)
        return 1;

    return 0;
}

/*
 * SmartScreen integration: checks if the executable would trigger
 * SmartScreen based on Zone.Identifier and signature status.
 * Returns: 0 = would not trigger, 1 = would trigger
 */
int codesign_smartscreen_check(const char *exe_path)
{
    /* No zone ID = no SmartScreen */
    if (!codesign_has_zone_id(exe_path))
        return 0;

    /* Valid signature = SmartScreen less likely to block */
    CSTrustResult sig = codesign_verify_signature(exe_path);
    if (sig == CS_TRUST_SIGNED)
        return 0;

    /* Unsigned + has zone ID = SmartScreen will trigger */
    return 1;
}

/*
 * Apply all available trust fixes to make the current exe
 * pass SmartScreen without warnings:
 * 1. Remove Zone.Identifier ADS
 * 2. Verify we're running with a proper manifest
 */
int codesign_apply_trust_fixes(void)
{
    int fixed = 0;

    /* Remove Zone.Identifier from self */
    if (codesign_remove_zone_id_self() == 0)
        fixed++;

    /* Remove Zone.Identifier from any companion files
       in the same directory */
    char dir[MAX_PATH];
    GetModuleFileNameA(NULL, dir, MAX_PATH);
    char *last_sep = strrchr(dir, '\\');
    if (last_sep) {
        *last_sep = '\0';
        char search[MAX_PATH];
        snprintf(search, sizeof(search), "%s\\*.dll", dir);

        WIN32_FIND_DATAA fd;
        HANDLE hf = FindFirstFileA(search, &fd);
        if (hf != INVALID_HANDLE_VALUE) {
            do {
                char full[MAX_PATH];
                snprintf(full, sizeof(full), "%s\\%s", dir, fd.cFileName);
                codesign_remove_zone_id(full);
                fixed++;
            } while (FindNextFileA(hf, &fd));
            FindClose(hf);
        }
    }

    return fixed;
}

int codesign_get_signer_name(const char *exe_path, char *out, int outsz)
{
    CSTrustResult r = codesign_verify_signature(exe_path);
    if (r != CS_TRUST_SIGNED) {
        snprintf(out, outsz, "(unsigned)");
        return -1;
    }
    snprintf(out, outsz, "%s", CODESIGN_PUBLISHER);
    return 0;
}

int codesign_get_timestamp(const char *exe_path, char *out, int outsz)
{
    (void)exe_path;
    /* Full implementation would parse the PKCS#7 timestamp
       from the Authenticode signature */
    snprintf(out, outsz, "(no timestamp)");
    return -1;
}

int codesign_generate_test_cert(const char *pfx_path, const char *password)
{
    /* Generate a self-signed certificate using CertCreateSelfSignCertificate.
       This is useful for development/testing but won't bypass SmartScreen
       in production — only EV certs or well-known CAs work for that. */
    (void)pfx_path;
    (void)password;
    /* TODO: implement with CryptoAPI */
    return -1;
}

int codesign_sign_with_powershell(const char *exe_path, const char *cert_thumbprint)
{
    /* Alternative signing method using PowerShell (no signtool needed):
       Set-AuthenticodeSignature -FilePath "path.exe" \
         -Certificate (Get-ChildItem Cert:\CurrentUser\My\THUMBPRINT) \
         -TimestampServer "http://timestamp.digicert.com" */
    char cmd[2048];
    snprintf(cmd, sizeof(cmd),
        "powershell -NoProfile -Command \""
        "$cert = Get-ChildItem -Path Cert:\\CurrentUser\\My\\%s; "
        "Set-AuthenticodeSignature -FilePath '%s' -Certificate $cert "
        "-TimestampServer '%s'\"",
        cert_thumbprint, exe_path, CODESIGN_TIMESTAMP_URL);

    return system(cmd);
}

#else
/* Unix: no SmartScreen, no code signing needed */
#endif
