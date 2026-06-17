/* puttyalt_layoutgrid.c - Compute uniform grid cell rectangles.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y, w, h; } GridCell;
/* Compute the rect of cell (col,row) in a gapped grid. */
GridCell lg_cell(int total_w, int total_h, int cols, int rows, int gap, int col, int row) {
    GridCell c = {0,0,0,0};
    if (cols <= 0 || rows <= 0) return c;
    int cw = (total_w - gap * (cols - 1)) / cols;
    int ch = (total_h - gap * (rows - 1)) / rows;
    c.x = col * (cw + gap);
    c.y = row * (ch + gap);
    c.w = cw; c.h = ch;
    return c;
}
int lg_index(int cols, int col, int row) { return row * cols + col; }
void lg_coords(int cols, int index, int *col, int *row) {
    if (cols <= 0) return;
    if (col) *col = index % cols;
    if (row) *row = index / cols;
}
