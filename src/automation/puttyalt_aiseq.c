/* puttyalt_aiseq.c - AI multi-step command sequence planner state. */
#include <string.h>
#include <stdio.h>
#define AS_MAX 16
#define AS_CMD 256
enum as_step { AS_PENDING=0, AS_RUNNING, AS_DONE, AS_FAILED, AS_SKIPPED };
typedef struct { char cmd[AS_CMD]; int state; int needs_confirm; } as_item;
typedef struct { as_item s[AS_MAX]; int n; int cursor; } AiSeq;
void aiseq_init(AiSeq *a) { if(a) memset(a,0,sizeof(*a)); }
int aiseq_add(AiSeq *a, const char *cmd, int needs_confirm) {
    if(!a||a->n>=AS_MAX||!cmd) return -1;
    as_item *it=&a->s[a->n]; snprintf(it->cmd,AS_CMD,"%s",cmd); it->needs_confirm=needs_confirm?1:0; return a->n++;
}
int aiseq_current(const AiSeq *a, char *out, int outlen) {
    if(!a||a->cursor>=a->n) return -1;
    if (out) snprintf(out,outlen,"%s",a->s[a->cursor].cmd); return a->cursor;
}
int aiseq_needs_confirm(const AiSeq *a) {
    if(!a||a->cursor>=a->n) return 0;
    return a->s[a->cursor].needs_confirm;
}
int aiseq_advance(AiSeq *a, int result) {
    if(!a||a->cursor>=a->n) return -1;
    a->s[a->cursor].state = result?AS_DONE:AS_FAILED;
    if (!result) return -1;
    /* halt on failure */
    a->cursor++; return a->cursor<a->n ? 0 : 1; /* 1 = sequence complete */
}
void aiseq_skip(AiSeq *a) { if(a&&a->cursor<a->n){ a->s[a->cursor].state=AS_SKIPPED; a->cursor++; } }
int aiseq_remaining(const AiSeq *a) { return a? (a->n - a->cursor):-1; }
