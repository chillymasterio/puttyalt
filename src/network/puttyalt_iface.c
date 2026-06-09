/* puttyalt_iface.c - Network interface enumeration + bind preference. */
#include <string.h>
#include <stdio.h>
#define IF_MAX 8
#define IF_NAME 32
#define IF_ADDR 48
typedef struct { char name[IF_NAME]; char addr[IF_ADDR]; int is_up; int is_loopback; int metric; } if_iface;
typedef struct { if_iface ifs[IF_MAX]; int n; int preferred; } NetIface;
void netiface_init(NetIface *n) { if(n){ memset(n,0,sizeof(*n)); n->preferred=-1; } }
int netiface_add(NetIface *n, const char *name, const char *addr, int is_up, int is_loopback, int metric) {
    if(!n||n->n>=IF_MAX||!name) return -1;
    if_iface *i=&n->ifs[n->n]; snprintf(i->name,IF_NAME,"%s",name); snprintf(i->addr,IF_ADDR,"%s",addr?addr:"");
    i->is_up=is_up?1:0; i->is_loopback=is_loopback?1:0; i->metric=metric; return n->n++;
}
int netiface_best(NetIface *n) {
    if(!n) return -1; int best=-1;
    for (int i=0;i<n->n;i++) {
        if (!n->ifs[i].is_up || n->ifs[i].is_loopback) continue;
        if (best<0 || n->ifs[i].metric<n->ifs[best].metric) best=i;
    }
    n->preferred=best; return best;
}
const char *netiface_bind_addr(const NetIface *n) {
    if(!n||n->preferred<0) return 0; return n->ifs[n->preferred].addr;
}
int netiface_count(const NetIface *n) { return n?n->n:-1; }
