#ifndef PUTTYALT_KEYGEN_H
#define PUTTYALT_KEYGEN_H

typedef enum {
    KEYGEN_RSA = 0,
    KEYGEN_ED25519,
    KEYGEN_ECDSA_256,
    KEYGEN_ECDSA_384,
    KEYGEN_ECDSA_521
} KeyGenType;

typedef struct KeyGenParams {
    KeyGenType type;
    int bits;              /* RSA key size */
    char comment[256];
    char passphrase[128];
    char output_path[512];
} KeyGenParams;

typedef struct KeyGenResult {
    int success;
    char public_key[4096];
    char fingerprint[128];
    char error[256];
} KeyGenResult;

int keygen_generate(const KeyGenParams *params, KeyGenResult *result);
int keygen_get_default_bits(KeyGenType type);
const char *keygen_type_name(KeyGenType type);

#endif
