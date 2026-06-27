/* puttyalt_tabexpand.c - Expand tab characters to spaces.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Expand tabs in `s` to spaces using `tabwidth` stops. */
int te2_expand(const char *s, int tabwidth, char *out, int outlen) {
    if (!s || !out || tabwidth <= 0) return -1;
    int o = 0, col = 0;
    for (; *s && o < outlen - 1; s++) {
        if (*s == '\t') {
            int spaces = tabwidth - (col % tabwidth);
            for (int i = 0; i < spaces && o < outlen - 1; i++) { out[o++] = ' '; col++; }
        } else if (*s == '\n') { out[o++] = '\n'; col = 0; }
        else { out[o++] = *s; col++; }
    }
    out[o] = 0;
    return o;
}
/* Compute the visual column after a string given tab stops. */
int te2_visual_col(const char *s, int tabwidth) {
    int col = 0;
    for (; *s; s++) {
        if (*s == '\t') col += tabwidth - (col % tabwidth);
        else if (*s == '\n') col = 0;
        else col++;
    }
    return col;
}
