#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "puttyalt_keygen.h"

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

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        strncpy(result->error, "Failed to execute ssh-keygen", sizeof(result->error) - 1);
        return -1;
    }

    char buf[4096];
    size_t total = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        total += strlen(buf);
    }
    int status = pclose(fp);

    if (status != 0) {
        strncpy(result->error, "ssh-keygen failed", sizeof(result->error) - 1);
        return -1;
    }

    /* Read public key */
    char pub_path[520];
    snprintf(pub_path, sizeof(pub_path), "%s.pub", params->output_path);
    fp = fopen(pub_path, "r");
    if (fp) {
        if (fgets(result->public_key, sizeof(result->public_key), fp))
            result->success = 1;
        fclose(fp);
    }

    return result->success ? 0 : -1;
}
