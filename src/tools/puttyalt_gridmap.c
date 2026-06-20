/* puttyalt_gridmap.c - 2D grid coordinate mapping.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Convert a flat index to (col,row) and back for a grid of `cols`. */
int gm3_index(int cols, int col, int row) { return cols > 0 ? row * cols + col : -1; }
void gm3_coords(int cols, int index, int *col, int *row) {
    if (cols <= 0) return;
    if (col) *col = index % cols;
    if (row) *row = index / cols;
}
/* Clamp coordinates into grid bounds. */
void gm3_clamp(int cols, int rows, int *col, int *row) {
    if (col) { if (*col < 0) *col = 0; if (*col >= cols) *col = cols-1; }
    if (row) { if (*row < 0) *row = 0; if (*row >= rows) *row = rows-1; }
}
int gm3_neighbors(int cols, int rows, int col, int row, int *out) {
    int n = 0;
    if (col > 0) out[n++] = gm3_index(cols, col-1, row);
    if (col < cols-1) out[n++] = gm3_index(cols, col+1, row);
    if (row > 0) out[n++] = gm3_index(cols, col, row-1);
    if (row < rows-1) out[n++] = gm3_index(cols, col, row+1);
    return n;
}
