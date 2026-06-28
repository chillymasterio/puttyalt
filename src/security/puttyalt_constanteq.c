/* puttyalt_constanteq.c - Constant-time comparison helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Compare two buffers in constant time. Returns 1 if equal. */
int ceq_memequal(const void *a, const void *b, int n) {
    const unsigned char *pa = a, *pb = b;
    unsigned char diff = 0;
    for (int i = 0; i < n; i++) diff |= pa[i] ^ pb[i];
    return diff == 0;
}
/* Constant-time string compare (compares up to max bytes). */
int ceq_strequal(const char *a, const char *b, int max) {
    unsigned char diff = 0;
    int ended = 0;
    for (int i = 0; i < max; i++) {
        unsigned char ca = ended ? 0 : (unsigned char)a[i];
        unsigned char cb = ended ? 0 : (unsigned char)b[i];
        diff |= ca ^ cb;
        if (ca == 0 || cb == 0) ended = 1;
    }
    return diff == 0;
}
