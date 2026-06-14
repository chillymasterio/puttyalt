/* puttyalt_columnize.c - Format strings into aligned columns.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
/* Lay out item count across `cols` columns; returns rows needed. */
int col_rows(int count, int cols) {
    if (cols <= 0 || count <= 0) return 0;
    return (count + cols - 1) / cols;
}
/* Compute padded width for an item set (max len + gap). */
int col_width(const char *const *items, int count, int gap) {
    int w = 0;
    for (int i = 0; i < count; i++) {
        int l = (int)strlen(items[i]);
        if (l > w) w = l;
    }
    return w + (gap > 0 ? gap : 1);
}
int col_pad(const char *s, int width, char *out, int outlen) {
    int n = (int)strlen(s);
    int o = 0;
    for (; o < n && o < outlen - 1; o++) out[o] = s[o];
    while (o < width && o < outlen - 1) out[o++] = ' ';
    out[o] = 0;
    return o;
}
