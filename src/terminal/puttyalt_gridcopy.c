/* puttyalt_gridcopy.c - Copy/scroll rectangular cell regions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
/* Scroll a region of `rows` x `cols` up by `n` lines (cell ids in a flat buf). */
int gc2_scroll_up(int *cells, int rows, int cols, int n, int fill) {
    if (!cells || rows <= 0 || cols <= 0 || n <= 0) return -1;
    if (n >= rows) { for (int i = 0; i < rows*cols; i++) cells[i] = fill; return 0; }
    memmove(cells, cells + n*cols, (size_t)(rows - n) * cols * sizeof(int));
    for (int i = (rows - n)*cols; i < rows*cols; i++) cells[i] = fill;
    return 0;
}
int gc2_scroll_down(int *cells, int rows, int cols, int n, int fill) {
    if (!cells || rows <= 0 || cols <= 0 || n <= 0) return -1;
    if (n >= rows) { for (int i = 0; i < rows*cols; i++) cells[i] = fill; return 0; }
    memmove(cells + n*cols, cells, (size_t)(rows - n) * cols * sizeof(int));
    for (int i = 0; i < n*cols; i++) cells[i] = fill;
    return 0;
}
