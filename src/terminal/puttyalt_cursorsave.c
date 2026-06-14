/* puttyalt_cursorsave.c - Save/restore cursor state stack (DECSC).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CUR_DEPTH 8
typedef struct { int row, col, fg, bg, attr; } CurState;
typedef struct { CurState stk[CUR_DEPTH]; int sp; } CurStack;
void cursave_init(CurStack *s) { if (s) s->sp = 0; }
int cursave_push(CurStack *s, int row, int col, int fg, int bg, int attr) {
    if (!s || s->sp >= CUR_DEPTH) return -1;
    s->stk[s->sp].row = row; s->stk[s->sp].col = col;
    s->stk[s->sp].fg = fg; s->stk[s->sp].bg = bg; s->stk[s->sp].attr = attr;
    s->sp++;
    return 0;
}
int cursave_pop(CurStack *s, int *row, int *col, int *fg, int *bg, int *attr) {
    if (!s || s->sp <= 0) return -1;
    s->sp--;
    if (row) *row = s->stk[s->sp].row;
    if (col) *col = s->stk[s->sp].col;
    if (fg) *fg = s->stk[s->sp].fg;
    if (bg) *bg = s->stk[s->sp].bg;
    if (attr) *attr = s->stk[s->sp].attr;
    return 0;
}
