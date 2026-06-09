/* puttyalt_chainrun.c - Command chain runner (&&, ||, ; operators). */
#include <string.h>
#include <stdio.h>
#define CR_MAX 16
#define CR_CMD 160
enum cr_op { CR_SEQ=0, CR_AND=1, CR_OR=2 };
typedef struct { char cmd[CR_CMD]; int op; int executed; int exit_code; } cr_step;
typedef struct { cr_step steps[CR_MAX]; int n; int cursor; } ChainRun;
void chainrun_init(ChainRun *c) { if(c) memset(c,0,sizeof(*c)); }
int chainrun_add(ChainRun *c, const char *cmd, int op) {
    if(!c||c->n>=CR_MAX||!cmd) return -1;
    cr_step *s=&c->steps[c->n++]; snprintf(s->cmd,CR_CMD,"%s",cmd); s->op=op; s->exit_code=-1; return 0;
}
int chainrun_next(ChainRun *c) {
    if(!c||c->cursor>=c->n) return -1;
    /* check if previous result allows this step */
    if (c->cursor>0) {
        int prev_code=c->steps[c->cursor-1].exit_code;
        int op=c->steps[c->cursor].op;
        if (op==CR_AND && prev_code!=0) { c->cursor++; return chainrun_next(c); } /* skip */
        if (op==CR_OR && prev_code==0) { c->cursor++; return chainrun_next(c); } /* skip */
    }
    return c->cursor;
}
int chainrun_complete(ChainRun *c, int exit_code) {
    if(!c||c->cursor>=c->n) return -1;
    c->steps[c->cursor].executed=1; c->steps[c->cursor].exit_code=exit_code; c->cursor++;
    return c->cursor>=c->n?1:0;
}
const char *chainrun_current(const ChainRun *c) {
    return (c&&c->cursor<c->n)?c->steps[c->cursor].cmd:0;
}
int chainrun_count(const ChainRun *c) { return c?c->n:-1; }
