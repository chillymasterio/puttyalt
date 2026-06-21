/* puttyalt_altbuffer.c - Manage alternate screen buffer switching.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int active; int saved_row, saved_col; int main_row, main_col; } AltBuffer;
void ab2_init(AltBuffer *a) { if (a) { a->active = 0; a->saved_row = a->saved_col = 0; a->main_row = a->main_col = 0; } }
/* Switch to alternate buffer, saving main cursor. */
void ab2_enter(AltBuffer *a, int cur_row, int cur_col) {
    if (!a || a->active) return;
    a->main_row = cur_row; a->main_col = cur_col;
    a->active = 1; a->saved_row = 0; a->saved_col = 0;
}
/* Switch back to main buffer; outputs restored cursor. */
void ab2_leave(AltBuffer *a, int *row, int *col) {
    if (!a || !a->active) return;
    a->active = 0;
    if (row) *row = a->main_row;
    if (col) *col = a->main_col;
}
int ab2_is_alt(const AltBuffer *a) { return a && a->active; }
