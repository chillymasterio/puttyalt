/* puttyalt_dirtyrect.c - Coalesce dirty rectangles for redraw.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x0, y0, x1, y1; int valid; } DirtyRect;
void dr4_init(DirtyRect *d) { if (d) d->valid = 0; }
void dr4_mark(DirtyRect *d, int x, int y) {
    if (!d) return;
    if (!d->valid) { d->x0 = d->x1 = x; d->y0 = d->y1 = y; d->valid = 1; }
    else {
        if (x < d->x0) d->x0 = x;
        if (x > d->x1) d->x1 = x;
        if (y < d->y0) d->y0 = y;
        if (y > d->y1) d->y1 = y;
    }
}
void dr4_mark_rect(DirtyRect *d, int x0, int y0, int x1, int y1) {
    dr4_mark(d, x0, y0); dr4_mark(d, x1, y1);
}
int dr4_area(const DirtyRect *d) {
    if (!d || !d->valid) return 0;
    return (d->x1 - d->x0 + 1) * (d->y1 - d->y0 + 1);
}
void dr4_clear(DirtyRect *d) { if (d) d->valid = 0; }
