/* puttyalt_charwidth.c - East-Asian / control char display width.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
int cw_width(uint32_t cp) {
    if (cp == 0) return 0;
    if (cp < 32 || (cp >= 0x7f && cp < 0xa0)) return -1; /* control */
    /* combining marks */
    if ((cp >= 0x0300 && cp <= 0x036f) || (cp >= 0x1ab0 && cp <= 0x1aff) ||
        (cp >= 0x20d0 && cp <= 0x20ff) || (cp >= 0xfe20 && cp <= 0xfe2f))
        return 0;
    /* common wide ranges */
    if ((cp >= 0x1100 && cp <= 0x115f) || /* Hangul Jamo */
        (cp >= 0x2e80 && cp <= 0xa4cf) || /* CJK */
        (cp >= 0xac00 && cp <= 0xd7a3) || /* Hangul syllables */
        (cp >= 0xf900 && cp <= 0xfaff) || /* CJK compat */
        (cp >= 0xff00 && cp <= 0xff60) || /* fullwidth */
        (cp >= 0x1f300 && cp <= 0x1faff)) /* emoji/symbols */
        return 2;
    return 1;
}
int cw_string_width(const uint32_t *cps, int n) {
    int w = 0;
    for (int i = 0; i < n; i++) { int x = cw_width(cps[i]); if (x > 0) w += x; }
    return w;
}
