/* puttyalt_hmacutil.c - HMAC key padding helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Prepare ipad/opad key blocks for HMAC given a key. blocksize typically 64. */
int hmu_pad(const unsigned char *key, int keylen, int blocksize,
            unsigned char *ipad, unsigned char *opad) {
    if (!key || !ipad || !opad || blocksize <= 0 || blocksize > 128) return -1;
    unsigned char k[128];
    memset(k, 0, blocksize);
    if (keylen > blocksize) keylen = blocksize; /* caller should hash long keys first */
    memcpy(k, key, keylen);
    for (int i = 0; i < blocksize; i++) { ipad[i] = k[i] ^ 0x36; opad[i] = k[i] ^ 0x5c; }
    return 0;
}
