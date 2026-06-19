/* puttyalt_line2.c - Bresenham line point generation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y; } LinePt;
/* Generate Bresenham line points; writes up to maxpts; returns count. */
int ln2_points(int x0, int y0, int x1, int y1, LinePt *out, int maxpts) {
    if (!out) return -1;
    int dx = x1>x0?x1-x0:x0-x1;
    int dy = y1>y0?y1-y0:y0-y1;
    int sx = x0<x1?1:-1, sy = y0<y1?1:-1;
    int err = dx - dy;
    int n = 0;
    while (n < maxpts) {
        out[n].x = x0; out[n].y = y0; n++;
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2*err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
    return n;
}
