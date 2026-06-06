/* puttyalt_statemachine.c - Generic state machine for automation flows. */
#include <string.h>
#include <stdio.h>
#define SM_STATES 16
#define SM_TRANS 48
#define SM_NAME 32
typedef struct { char event[SM_NAME]; int from; int to; } sm_trans;
typedef struct { char names[SM_STATES][SM_NAME]; int nstates; sm_trans trans[SM_TRANS]; int ntrans; int current; } StateMachine;
void statemachine_init(StateMachine *s) { if(s){ memset(s,0,sizeof(*s)); s->current=-1; } }
int statemachine_add_state(StateMachine *s, const char *name) {
    if(!s||s->nstates>=SM_STATES||!name) return -1;
    snprintf(s->names[s->nstates],SM_NAME,"%s",name);
    if (s->current<0) s->current=s->nstates;
    return s->nstates++;
}
int statemachine_add_transition(StateMachine *s, const char *event, int from, int to) {
    if(!s||s->ntrans>=SM_TRANS||!event||from<0||from>=s->nstates||to<0||to>=s->nstates) return -1;
    sm_trans *t=&s->trans[s->ntrans++]; snprintf(t->event,SM_NAME,"%s",event); t->from=from; t->to=to; return 0;
}
int statemachine_fire(StateMachine *s, const char *event) {
    if(!s||!event) return -1;
    for (int i=0;i<s->ntrans;i++) if (s->trans[i].from==s->current && strcmp(s->trans[i].event,event)==0) {
        s->current=s->trans[i].to; return s->current;
    }
    return -1; /* no valid transition */
}
int statemachine_current(const StateMachine *s) { return s?s->current:-1; }
const char *statemachine_current_name(const StateMachine *s) {
    return (s&&s->current>=0&&s->current<s->nstates)?s->names[s->current]:0;
}
