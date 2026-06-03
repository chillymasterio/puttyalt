/* puttyalt_termstate2.c - Terminal state v2: modes bitmap + saved cursor stack. */
#include <string.h>
#define TS_STACK 8
enum ts_mode { TS_WRAP=1, TS_INSERT=2, TS_APPCURSOR=4, TS_APPKEYPAD=8, TS_MOUSE=16, TS_BRACKETED=32, TS_FOCUS=64 };
typedef struct { int x, y, attr; } ts_cursor;
typedef struct { int modes; ts_cursor stack[TS_STACK]; int sp; int cx, cy, attr; } TermState2;
void termstate2_init(TermState2 *t) { if(t){ memset(t,0,sizeof(*t)); t->modes=TS_WRAP; } }
void termstate2_set_mode(TermState2 *t, int mode, int on) { if(t){ if(on) t->modes|=mode; else t->modes&=~mode; } }
int termstate2_has_mode(const TermState2 *t, int mode) { return (t && (t->modes&mode))?1:0; }
int termstate2_save_cursor(TermState2 *t) {
    if(!t||t->sp>=TS_STACK) return -1;
    t->stack[t->sp].x=t->cx; t->stack[t->sp].y=t->cy; t->stack[t->sp].attr=t->attr; t->sp++; return 0;
}
int termstate2_restore_cursor(TermState2 *t) {
    if(!t||t->sp==0) return -1;
    t->sp--; t->cx=t->stack[t->sp].x; t->cy=t->stack[t->sp].y; t->attr=t->stack[t->sp].attr; return 0;
}
void termstate2_move(TermState2 *t, int x, int y) { if(t){ t->cx=x; t->cy=y; } }
int termstate2_stack_depth(const TermState2 *t) { return t?t->sp:-1; }
