/* puttyalt_scrollregion.c - Manage DECSTBM scroll regions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int top, bottom, rows; } ScrollRegion;
void scr_init(ScrollRegion *s, int rows) {
    if (!s) return;
    s->rows = rows > 0 ? rows : 24;
    s->top = 0;
    s->bottom = s->rows - 1;
}
int scr_set(ScrollRegion *s, int top, int bottom) {
    if (!s) return -1;
    if (top < 0) top = 0;
    if (bottom >= s->rows) bottom = s->rows - 1;
    if (top >= bottom) return -1;
    s->top = top; s->bottom = bottom;
    return 0;
}
int scr_in_region(const ScrollRegion *s, int row) {
    return s && row >= s->top && row <= s->bottom;
}
void scr_reset(ScrollRegion *s) { if (s) { s->top = 0; s->bottom = s->rows - 1; } }
int scr_height(const ScrollRegion *s) { return s ? s->bottom - s->top + 1 : 0; }
