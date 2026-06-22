/* puttyalt_colorpick.c - HSV/RGB conversion for color picker.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void cpk_hsv_to_rgb(int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b) {
    /* h:0-359 s:0-100 v:0-100 */
    if (s == 0) { *r = *g = *b = (uint8_t)(v * 255 / 100); return; }
    int region = h / 60;
    int rem = (h - region * 60) * 255 / 60;
    int p = v * (100 - s) * 255 / 10000;
    int q = v * (100 - (s * rem / 255)) * 255 / 10000;
    int t = v * (100 - (s * (255 - rem) / 255)) * 255 / 10000;
    int vv = v * 255 / 100;
    switch (region) {
        case 0: *r=vv; *g=t; *b=p; break;
        case 1: *r=q; *g=vv; *b=p; break;
        case 2: *r=p; *g=vv; *b=t; break;
        case 3: *r=p; *g=q; *b=vv; break;
        case 4: *r=t; *g=p; *b=vv; break;
        default: *r=vv; *g=p; *b=q; break;
    }
}
int cpk_rgb_to_hue(uint8_t r, uint8_t g, uint8_t b) {
    int mx = r>g?(r>b?r:b):(g>b?g:b);
    int mn = r<g?(r<b?r:b):(g<b?g:b);
    int d = mx - mn;
    if (d == 0) return 0;
    int h;
    if (mx == r) h = 60 * (((g - b) * 100 / d)) / 100;
    else if (mx == g) h = 60 * (2 + (b - r) * 100 / d / 100);
    else h = 60 * (4 + (r - g) * 100 / d / 100);
    if (h < 0) h += 360;
    return h;
}
