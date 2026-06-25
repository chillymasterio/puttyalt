/* puttyalt_fsm2.c - Named-state finite state machine.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define FSM_STATES 16
#define FSM_TRANS 48
typedef struct { char from[16]; char event[16]; char to[16]; } FsmTrans;
typedef struct { FsmTrans trans[FSM_TRANS]; int nt; char state[16]; } Fsm2;
void fsm_init(Fsm2 *m, const char *initial) {
    if (!m) return;
    m->nt = 0;
    strncpy(m->state, initial ? initial : "", 15); m->state[15] = 0;
}
int fsm_add(Fsm2 *m, const char *from, const char *event, const char *to) {
    if (!m || m->nt >= FSM_TRANS) return -1;
    strncpy(m->trans[m->nt].from, from, 15); m->trans[m->nt].from[15] = 0;
    strncpy(m->trans[m->nt].event, event, 15); m->trans[m->nt].event[15] = 0;
    strncpy(m->trans[m->nt].to, to, 15); m->trans[m->nt].to[15] = 0;
    m->nt++;
    return 0;
}
int fsm_fire(Fsm2 *m, const char *event) {
    if (!m || !event) return -1;
    for (int i = 0; i < m->nt; i++)
        if (strcmp(m->trans[i].from, m->state) == 0 && strcmp(m->trans[i].event, event) == 0) {
            strncpy(m->state, m->trans[i].to, 15); m->state[15] = 0;
            return 0;
        }
    return -1;
}
const char *fsm_state(const Fsm2 *m) { return m ? m->state : ""; }
