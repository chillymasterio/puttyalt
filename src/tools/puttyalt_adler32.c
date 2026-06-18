/* puttyalt_adler32.c - Adler-32 checksum.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#define AD_MOD 65521
uint32_t ad_compute(const unsigned char *d, int n) {
    uint32_t a = 1, b = 0;
    for (int i = 0; i < n; i++) { a = (a + d[i]) % AD_MOD; b = (b + a) % AD_MOD; }
    return (b << 16) | a;
}
uint32_t ad_update(uint32_t adler, const unsigned char *d, int n) {
    uint32_t a = adler & 0xFFFF, b = (adler >> 16) & 0xFFFF;
    for (int i = 0; i < n; i++) { a = (a + d[i]) % AD_MOD; b = (b + a) % AD_MOD; }
    return (b << 16) | a;
}
