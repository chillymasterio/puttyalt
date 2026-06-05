/* puttyalt_natpunch.c - NAT traversal hole-punching state (STUN-style candidate pairs). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define NP_MAX 16
#define NP_ADDR 64
enum np_state { NP_GATHERING=0, NP_CHECKING, NP_CONNECTED, NP_FAILED };
typedef struct { char local[NP_ADDR]; char remote[NP_ADDR]; int priority; int state; int rtt; } np_pair;
typedef struct { np_pair pairs[NP_MAX]; int n; int state; int selected; } NatPunch;
void natpunch_init(NatPunch *np) { if(np){ memset(np,0,sizeof(*np)); np->state=NP_GATHERING; np->selected=-1; } }
int natpunch_add_pair(NatPunch *np, const char *local, const char *remote, int priority) {
    if(!np||np->n>=NP_MAX||!local||!remote) return -1;
    np_pair *p=&np->pairs[np->n]; snprintf(p->local,NP_ADDR,"%s",local); snprintf(p->remote,NP_ADDR,"%s",remote);
    p->priority=priority; p->state=NP_CHECKING; return np->n++;
}
int natpunch_check_result(NatPunch *np, int idx, int success, int rtt) {
    if(!np||idx<0||idx>=np->n) return -1;
    if (success) { np->pairs[idx].state=NP_CONNECTED; np->pairs[idx].rtt=rtt;
        if (np->selected<0 || np->pairs[idx].priority>np->pairs[np->selected].priority) { np->selected=idx; np->state=NP_CONNECTED; }
    } else np->pairs[idx].state=NP_FAILED;
    return 0;
}
int natpunch_best_pair(const NatPunch *np) { return np?np->selected:-1; }
int natpunch_state(const NatPunch *np) { return np?np->state:-1; }
