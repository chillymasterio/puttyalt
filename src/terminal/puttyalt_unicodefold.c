/* puttyalt_unicodefold.c - Simple Unicode case folding for ASCII+Latin1.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t uf_fold(uint32_t cp) {
    if (cp >= 'A' && cp <= 'Z') return cp + 32;
    if (cp >= 0xC0 && cp <= 0xDE && cp != 0xD7) return cp + 32; /* Latin-1 uppercase */
    return cp;
}
int uf_equal_fold(const uint32_t *a, const uint32_t *b, int n) {
    for (int i = 0; i < n; i++) if (uf_fold(a[i]) != uf_fold(b[i])) return 0;
    return 1;
}
int uf_is_upper(uint32_t cp) {
    return (cp >= 'A' && cp <= 'Z') || (cp >= 0xC0 && cp <= 0xDE && cp != 0xD7);
}
