/* puttyalt_colorparse.c - Parse #RRGGBB / rgb() color strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
int cp2_hex(const char *s, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (!s) return -1;
    if (*s == '#') s++;
    unsigned rv, gv, bv;
    if (strlen(s) >= 6 && sscanf(s, "%02x%02x%02x", &rv, &gv, &bv) == 3) {
        *r = (uint8_t)rv; *g = (uint8_t)gv; *b = (uint8_t)bv; return 0;
    }
    if (strlen(s) >= 3) {
        unsigned x, y, z;
        if (sscanf(s, "%01x%01x%01x", &x, &y, &z) == 3) {
            *r = (uint8_t)(x*17); *g = (uint8_t)(y*17); *b = (uint8_t)(z*17); return 0;
        }
    }
    return -1;
}
int cp2_rgbfn(const char *s, uint8_t *r, uint8_t *g, uint8_t *b) {
    unsigned rv, gv, bv;
    if (s && sscanf(s, "rgb(%u,%u,%u)", &rv, &gv, &bv) == 3) {
        *r=(uint8_t)(rv>255?255:rv); *g=(uint8_t)(gv>255?255:gv); *b=(uint8_t)(bv>255?255:bv); return 0;
    }
    return -1;
}
int cp2_luminance(uint8_t r, uint8_t g, uint8_t b) {
    return (int)((r * 299 + g * 587 + b * 114) / 1000);
}
