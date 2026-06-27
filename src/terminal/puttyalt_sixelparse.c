/* puttyalt_sixelparse.c - Parse Sixel graphics dimensions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int width; int height; int color_count; int valid; } SixelInfo;
/* Scan a Sixel data string to estimate dimensions. */
int sxp_measure(const char *data, int n, SixelInfo *out) {
    if (!data || !out) return -1;
    out->width = 0; out->height = 0; out->color_count = 0; out->valid = 0;
    int x = 0, band = 0, max_x = 0;
    for (int i = 0; i < n; i++) {
        char c = data[i];
        if (c == '#') out->color_count++;
        else if (c == '$') { if (x > max_x) max_x = x; x = 0; }
        else if (c == '-') { if (x > max_x) max_x = x; x = 0; band++; }
        else if (c >= '?' && c <= '~') x++;
    }
    if (x > max_x) max_x = x;
    out->width = max_x;
    out->height = (band + 1) * 6;
    out->valid = 1;
    return 0;
}
int sxp_has_sixel(const char *data, int n) {
    for (int i = 0; i + 1 < n; i++) if (data[i] == 0x1b && data[i+1] == 'P') return 1;
    return 0;
}
