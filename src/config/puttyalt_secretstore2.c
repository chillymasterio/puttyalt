/* puttyalt_secretstore2.c - Encrypted secret store with simple XOR keystream.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
/* Obfuscate/deobfuscate a value with a keystream (NOT strong crypto;
   defense-in-depth against casual disk inspection). */
void ss5_transform(const unsigned char *in, int n, uint64_t key, unsigned char *out) {
    uint64_t k = key ? key : 0x123456789abcdefULL;
    for (int i = 0; i < n; i++) {
        k = k * 6364136223846793005ULL + 1442695040888963407ULL;
        out[i] = in[i] ^ (unsigned char)(k >> 33);
    }
}
/* Derive a store key from a master passphrase. */
uint64_t ss5_keyfrom(const char *passphrase) {
    uint64_t h = 1469598103934665603ULL;
    if (passphrase) for (const char *p = passphrase; *p; p++) { h ^= (unsigned char)*p; h *= 1099511628211ULL; }
    return h;
}
