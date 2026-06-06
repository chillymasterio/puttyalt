/* puttyalt_titlestack.c - Terminal title stack (XTPUSHTITLE/XTPOPTITLE). */
#include <string.h>
#include <stdio.h>
#define TS_DEPTH 8
#define TS_TITLE 128
typedef struct { char stack[TS_DEPTH][TS_TITLE]; int sp; char current[TS_TITLE]; } TitleStack;
void titlestack_init(TitleStack *t) { if(t){ memset(t,0,sizeof(*t)); } }
void titlestack_set(TitleStack *t, const char *title) { if(t) snprintf(t->current,TS_TITLE,"%s",title?title:""); }
int titlestack_push(TitleStack *t) {
    if(!t||t->sp>=TS_DEPTH) return -1;
    snprintf(t->stack[t->sp++],TS_TITLE,"%s",t->current); return 0;
}
int titlestack_pop(TitleStack *t) {
    if(!t||t->sp==0) return -1;
    snprintf(t->current,TS_TITLE,"%s",t->stack[--t->sp]); return 0;
}
const char *titlestack_current(const TitleStack *t) { return t?t->current:""; }
int titlestack_depth(const TitleStack *t) { return t?t->sp:-1; }
