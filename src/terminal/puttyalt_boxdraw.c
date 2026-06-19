/* puttyalt_boxdraw.c - Build box-drawing frames.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#include <string.h>
/* Choose a box-drawing glyph (unicode codepoint) for a corner/edge. */
unsigned bx2_glyph(int top, int bottom, int left, int right) {
    /* bit combos -> line characters */
    if (left && right && !top && !bottom) return 0x2500; /* horizontal */
    if (top && bottom && !left && !right) return 0x2502; /* vertical */
    if (right && bottom && !left && !top) return 0x250C; /* top-left */
    if (left && bottom && !right && !top) return 0x2510; /* top-right */
    if (right && top && !left && !bottom) return 0x2514; /* bottom-left */
    if (left && top && !right && !bottom) return 0x2518; /* bottom-right */
    if (top && bottom && left && right) return 0x253C;   /* cross */
    return ' ';
}
/* Fill ASCII box border chars into a w*h grid (top-left origin). */
int bx2_ascii(int w, int h, char *grid) {
    if (!grid || w < 2 || h < 2) return -1;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            char c = ' ';
            int edge_t = (y==0), edge_b = (y==h-1), edge_l = (x==0), edge_r = (x==w-1);
            if ((edge_t||edge_b) && (edge_l||edge_r)) c = '+';
            else if (edge_t||edge_b) c = '-';
            else if (edge_l||edge_r) c = '|';
            grid[y*w+x] = c;
        }
    return 0;
}
