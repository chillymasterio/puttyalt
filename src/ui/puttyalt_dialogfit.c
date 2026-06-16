/* puttyalt_dialogfit.c - Compute centered dialog geometry.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int x, y, w, h; } DlgRect;
DlgRect df_center(int parent_w, int parent_h, int dlg_w, int dlg_h) {
    DlgRect r;
    r.w = dlg_w; r.h = dlg_h;
    r.x = (parent_w - dlg_w) / 2;
    r.y = (parent_h - dlg_h) / 2;
    if (r.x < 0) r.x = 0;
    if (r.y < 0) r.y = 0;
    return r;
}
/* Clamp a dialog to stay fully on screen. */
DlgRect df_clamp(DlgRect r, int screen_w, int screen_h) {
    if (r.w > screen_w) r.w = screen_w;
    if (r.h > screen_h) r.h = screen_h;
    if (r.x + r.w > screen_w) r.x = screen_w - r.w;
    if (r.y + r.h > screen_h) r.y = screen_h - r.h;
    if (r.x < 0) r.x = 0;
    if (r.y < 0) r.y = 0;
    return r;
}
