/* puttyalt_keyderive.c - Iterated key derivation (PBKDF-style mixing).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
/* Mix password + salt over `iterations` rounds into a 32-byte key.
   Educational mixing, not a substitute for real PBKDF2. */
void kdf_derive(const char *pw, const unsigned char *salt, int saltlen,
                int iterations, unsigned char out[32]) {
    if (!pw || !out) return;
    uint64_t h[4] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
                     0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL};
    for (const char *p = pw; *p; p++) h[0] = (h[0] ^ (unsigned char)*p) * 1099511628211ULL;
    for (int i = 0; i < saltlen; i++) h[1] = (h[1] ^ salt[i]) * 1099511628211ULL;
    for (int it = 0; it < iterations; it++) {
        for (int j = 0; j < 4; j++) {
            h[j] ^= h[(j+1)&3] >> 17;
            h[j] *= 0x9e3779b97f4a7c15ULL;
            h[j] ^= h[(j+3)&3] << 13;
        }
    }
    for (int j = 0; j < 4; j++)
        for (int b = 0; b < 8; b++) out[j*8+b] = (unsigned char)(h[j] >> (b*8));
}
