/* puttyalt_hashcmp.c - Compare hash digests safely.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int hcmp_equal(const uint8_t *a, const uint8_t *b, int n) {
    uint8_t diff = 0;
    for (int i=0;i<n;i++) diff |= a[i]^b[i];
    return diff == 0;
}
int hcmp_hexequal(const char *a, const char *b) {
    int diff = 0, i = 0;
    for (; a[i] && b[i]; i++) { char ca=a[i]|0x20, cb=b[i]|0x20; diff |= ca^cb; }
    diff |= a[i] ^ b[i];
    return diff == 0;
}
