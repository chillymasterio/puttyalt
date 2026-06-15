/* puttyalt_linewrap2.c - Soft-wrap reflow on terminal resize.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Compute how many display rows a logical line of `len` chars takes at width. */
int lw2_rows(int len, int width) {
    if (width <= 0) return 1;
    if (len <= 0) return 1;
    return (len + width - 1) / width;
}
/* Map a logical column to (row, col) within the wrapped display. */
void lw2_map(int logical_col, int width, int *row, int *col) {
    if (width <= 0) { if (row) *row = 0; if (col) *col = logical_col; return; }
    if (row) *row = logical_col / width;
    if (col) *col = logical_col % width;
}
/* Total display rows for a buffer of line lengths after a resize. */
int lw2_total_rows(const int *line_lens, int n, int width) {
    int total = 0;
    for (int i = 0; i < n; i++) total += lw2_rows(line_lens[i], width);
    return total;
}
