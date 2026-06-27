/* puttyalt_cursorshape.c - Manage cursor shape and visibility.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { CUR_BLOCK, CUR_UNDERLINE, CUR_BAR };
typedef struct { int shape; int blinking; int visible; } CursorShape;
void cs4_init(CursorShape *c) { if (c) { c->shape = CUR_BLOCK; c->blinking = 1; c->visible = 1; } }
/* Apply a DECSCUSR parameter (0-6). */
void cs4_decscusr(CursorShape *c, int param) {
    if (!c) return;
    switch (param) {
        case 0: case 1: c->shape = CUR_BLOCK; c->blinking = 1; break;
        case 2: c->shape = CUR_BLOCK; c->blinking = 0; break;
        case 3: c->shape = CUR_UNDERLINE; c->blinking = 1; break;
        case 4: c->shape = CUR_UNDERLINE; c->blinking = 0; break;
        case 5: c->shape = CUR_BAR; c->blinking = 1; break;
        case 6: c->shape = CUR_BAR; c->blinking = 0; break;
    }
}
const char *cs4_name(const CursorShape *c) {
    if (!c) return "block";
    switch (c->shape) { case CUR_UNDERLINE: return "underline"; case CUR_BAR: return "bar"; default: return "block"; }
}
