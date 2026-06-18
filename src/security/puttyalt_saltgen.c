/* puttyalt_saltgen.c - Derive deterministic salts and stretch hashes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* FNV-1a based key stretching (NOT cryptographic; for nonces/derivation). */
uint64_t sg2_stretch(const char *input, int rounds) {
    if (!input) return 0;
    uint64_t h = 1469598103934665603ULL;
    for (const char *p = input; *p; p++) { h ^= (unsigned char)*p; h *= 1099511628211ULL; }
    for (int i = 0; i < rounds; i++) { h ^= h >> 33; h *= 0xff51afd7ed558ccdULL; h ^= h >> 33; }
    return h;
}
int sg2_salt_hex(uint64_t seed, char *out, int outlen) {
    static const char H[] = "0123456789abcdef";
    if (!out || outlen < 17) return -1;
    for (int i = 0; i < 16; i++) out[i] = H[(seed >> ((15-i)*4)) & 15];
    out[16] = 0;
    return 16;
}
