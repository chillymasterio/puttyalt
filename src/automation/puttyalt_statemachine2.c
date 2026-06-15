/* puttyalt_statemachine2.c - Generic transition-table state machine.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SM_MAX 32
typedef struct { int from, event, to; } SmRule;
typedef struct { SmRule rules[SM_MAX]; int nrules; int state; } StateMachine2;
void sm2_init(StateMachine2 *m, int initial) { if (m) { m->nrules = 0; m->state = initial; } }
int sm2_rule(StateMachine2 *m, int from, int event, int to) {
    if (!m || m->nrules >= SM_MAX) return -1;
    m->rules[m->nrules].from = from; m->rules[m->nrules].event = event; m->rules[m->nrules].to = to;
    m->nrules++;
    return 0;
}
/* Apply event; returns new state or -1 if no rule matches. */
int sm2_fire(StateMachine2 *m, int event) {
    if (!m) return -1;
    for (int i = 0; i < m->nrules; i++)
        if (m->rules[i].from == m->state && m->rules[i].event == event) { m->state = m->rules[i].to; return m->state; }
    return -1;
}
int sm2_state(const StateMachine2 *m) { return m ? m->state : -1; }
