/* puttyalt_wrapcursor.c - Track cursor wrapping at line ends.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int row, col, cols, rows; int pending_wrap; } WrapCursor;
void wcu_init(WrapCursor *c, int cols, int rows) {
    if (!c) return;
    c->cols = cols > 0 ? cols : 80; c->rows = rows > 0 ? rows : 24;
    c->row = c->col = 0; c->pending_wrap = 0;
}
/* Advance cursor by one printed cell, handling deferred wrap. */
void wcu_advance(WrapCursor *c) {
    if (!c) return;
    if (c->pending_wrap) { c->col = 0; c->row++; c->pending_wrap = 0; }
    c->col++;
    if (c->col >= c->cols) { c->col = c->cols - 1; c->pending_wrap = 1; }
    if (c->row >= c->rows) c->row = c->rows - 1;
}
void wcu_cr(WrapCursor *c) { if (c) { c->col = 0; c->pending_wrap = 0; } }
void wcu_lf(WrapCursor *c) { if (c) { c->row++; c->pending_wrap = 0; if (c->row >= c->rows) c->row = c->rows-1; } }
