/* puttyalt_runbook.c - Runbook executor: ordered steps with rollback markers. */
#include <string.h>
#include <stdio.h>
#define RB_MAX 32
#define RB_NAME 64
enum rb_status { RB_PENDING=0, RB_RUNNING, RB_DONE, RB_FAILED, RB_ROLLED_BACK };
typedef struct { char name[RB_NAME]; char rollback[RB_NAME]; int status; int critical; } rb_step;
typedef struct { rb_step steps[RB_MAX]; int n; int cursor; int failed_at; } Runbook;
void runbook_init(Runbook *r) { if(r){ memset(r,0,sizeof(*r)); r->failed_at=-1; } }
int runbook_add(Runbook *r, const char *name, const char *rollback, int critical) {
    if(!r||r->n>=RB_MAX||!name) return -1;
    rb_step *s=&r->steps[r->n]; snprintf(s->name,RB_NAME,"%s",name); snprintf(s->rollback,RB_NAME,"%s",rollback?rollback:"");
    s->critical=critical?1:0; return r->n++;
}
int runbook_next(Runbook *r) {
    if(!r||r->cursor>=r->n||r->failed_at>=0) return -1;
    r->steps[r->cursor].status=RB_RUNNING; return r->cursor;
}
int runbook_complete(Runbook *r, int success) {
    if(!r||r->cursor>=r->n) return -1;
    if (success) { r->steps[r->cursor].status=RB_DONE; r->cursor++; return r->cursor>=r->n?1:0; }
    r->steps[r->cursor].status=RB_FAILED; r->failed_at=r->cursor;
    return r->steps[r->cursor].critical ? -1 : 0; /* -1 = trigger rollback */
}
int runbook_rollback_plan(Runbook *r, int *out_idx, int cap) {
    if(!r||r->failed_at<0) return -1; int n=0;
    /* rollback completed steps in reverse */
    for (int i=r->failed_at-1;i>=0 && n<cap;i--) if (r->steps[i].status==RB_DONE && r->steps[i].rollback[0]) {
        out_idx[n++]=i; r->steps[i].status=RB_ROLLED_BACK;
    }
    return n;
}
int runbook_count(const Runbook *r) { return r?r->n:-1; }
