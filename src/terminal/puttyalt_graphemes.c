/* puttyalt_graphemes.c - Detect grapheme cluster boundaries (basic).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Heuristic: a combining mark or ZWJ does not start a new cluster. */
int gph_is_combining(uint32_t cp) {
    return (cp >= 0x0300 && cp <= 0x036F) || (cp >= 0x1DC0 && cp <= 0x1DFF) ||
           (cp >= 0x20D0 && cp <= 0x20FF) || (cp >= 0xFE20 && cp <= 0xFE2F);
}
int gph_is_zwj(uint32_t cp) { return cp == 0x200D; }
/* Should a break occur between prev and cur codepoints? */
int gph_boundary(uint32_t prev, uint32_t cur) {
    if (gph_is_combining(cur)) return 0;
    if (gph_is_zwj(prev) || gph_is_zwj(cur)) return 0;
    return 1;
}
/* Count grapheme clusters in a codepoint array. */
int gph_count(const uint32_t *cps, int n) {
    if (n <= 0) return 0;
    int count = 1;
    for (int i = 1; i < n; i++) if (gph_boundary(cps[i-1], cps[i])) count++;
    return count;
}
