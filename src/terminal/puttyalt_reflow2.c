/* puttyalt_reflow2.c - Reflow scrollback on width change.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Given a logical line length, distribute into wrapped segment lengths.
   Writes up to maxseg segment lengths; returns segment count. */
int rfl_segments(int line_len, int width, int *seg, int maxseg) {
    if (width <= 0 || !seg) return 0;
    if (line_len <= 0) { if (maxseg > 0) seg[0] = 0; return 1; }
    int n = 0, remaining = line_len;
    while (remaining > 0 && n < maxseg) {
        int take = remaining > width ? width : remaining;
        seg[n++] = take;
        remaining -= take;
    }
    return n;
}
/* Display row count for several lines after a width change. */
int rfl_height(const int *lens, int n, int width) {
    int h = 0;
    for (int i = 0; i < n; i++) {
        int rows = width > 0 ? (lens[i] + width - 1) / width : 1;
        h += rows < 1 ? 1 : rows;
    }
    return h;
}
