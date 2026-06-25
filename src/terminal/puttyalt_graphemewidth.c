/* puttyalt_graphemewidth.c - Compute display width of grapheme runs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int gw2_cp_width(uint32_t cp) {
    if (cp == 0) return 0;
    if (cp < 0x20 || (cp >= 0x7f && cp < 0xa0)) return 0;
    if ((cp >= 0x300 && cp <= 0x36F) || (cp >= 0x200B && cp <= 0x200F)) return 0;
    if ((cp >= 0x1100 && cp <= 0x115F) || (cp >= 0x2E80 && cp <= 0xA4CF) ||
        (cp >= 0xAC00 && cp <= 0xD7A3) || (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFF00 && cp <= 0xFF60) || (cp >= 0x1F300 && cp <= 0x1FAFF)) return 2;
    return 1;
}
int gw2_run_width(const uint32_t *cps, int n) {
    int w = 0;
    for (int i = 0; i < n; i++) w += gw2_cp_width(cps[i]);
    return w;
}
/* How many codepoints fit in `max_width` columns. */
int gw2_fit(const uint32_t *cps, int n, int max_width) {
    int w = 0, i = 0;
    for (; i < n; i++) { int cw = gw2_cp_width(cps[i]); if (w + cw > max_width) break; w += cw; }
    return i;
}
