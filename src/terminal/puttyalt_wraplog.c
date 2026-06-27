/* puttyalt_wraplog.c - Manage line-wrap continuation markers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define WL_MAX 256
typedef struct { unsigned char wrapped[WL_MAX/8]; int rows; } WrapLog;
void wl3_init(WrapLog *w, int rows) {
    if (!w) return;
    w->rows = (rows > 0 && rows <= WL_MAX) ? rows : WL_MAX;
    memset(w->wrapped, 0, sizeof w->wrapped);
}
void wl3_set_wrapped(WrapLog *w, int row, int wrapped) {
    if (!w || row < 0 || row >= w->rows) return;
    if (wrapped) w->wrapped[row/8] |= (unsigned char)(1 << (row % 8));
    else w->wrapped[row/8] &= (unsigned char)~(1 << (row % 8));
}
int wl3_is_wrapped(const WrapLog *w, int row) {
    if (!w || row < 0 || row >= w->rows) return 0;
    return (w->wrapped[row/8] >> (row % 8)) & 1;
}
/* Find the start row of the logical line containing `row`. */
int wl3_line_start(const WrapLog *w, int row) {
    if (!w) return row;
    while (row > 0 && wl3_is_wrapped(w, row - 1)) row--;
    return row;
}
