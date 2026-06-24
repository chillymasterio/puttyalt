/* puttyalt_unicodesafe.c - Detect unsafe Unicode (bidi/homoglyph).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
/* Detect bidirectional control characters used in Trojan-source attacks. */
int us2_is_bidi_control(uint32_t cp) {
    return (cp >= 0x202A && cp <= 0x202E) || (cp >= 0x2066 && cp <= 0x2069) ||
           cp == 0x200E || cp == 0x200F;
}
/* Detect zero-width characters often used to hide content. */
int us2_is_zero_width(uint32_t cp) {
    return cp == 0x200B || cp == 0x200C || cp == 0x200D || cp == 0xFEFF || cp == 0x2060;
}
/* Scan an array for suspicious codepoints; returns count found. */
int us2_scan(const uint32_t *cps, int n) {
    int found = 0;
    for (int i = 0; i < n; i++) if (us2_is_bidi_control(cps[i]) || us2_is_zero_width(cps[i])) found++;
    return found;
}
