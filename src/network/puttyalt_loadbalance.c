/* puttyalt_loadbalance.c - Connection load balancer across endpoints. */
#include <string.h>
#include <stdio.h>
#define LB_MAX 16
#define LB_HOST 64
enum lb_strategy { LB_ROUND_ROBIN=0, LB_LEAST_CONN=1, LB_WEIGHTED=2, LB_LATENCY=3 };
typedef struct { char host[LB_HOST]; int weight; int active_conns; int latency_ms; int healthy; } lb_endpoint;
typedef struct { lb_endpoint e[LB_MAX]; int n; int strategy; int rr_index; } LoadBalance;
void loadbalance_init(LoadBalance *l, int strategy) {
    if(!l) return; memset(l,0,sizeof(*l)); l->strategy=strategy;
}
int loadbalance_add(LoadBalance *l, const char *host, int weight) {
    if(!l||l->n>=LB_MAX||!host) return -1;
    lb_endpoint *e=&l->e[l->n]; snprintf(e->host,LB_HOST,"%s",host); e->weight=weight>0?weight:1; e->healthy=1; e->latency_ms=100;
    return l->n++;
}
void loadbalance_update(LoadBalance *l, int idx, int healthy, int latency_ms) {
    if(!l||idx<0||idx>=l->n) return; l->e[idx].healthy=healthy?1:0; if(healthy) l->e[idx].latency_ms=latency_ms;
}
int loadbalance_select(LoadBalance *l) {
    if(!l||l->n==0) return -1;
    int best=-1;
    for (int tries=0;tries<l->n;tries++) {
        int idx;
        if (l->strategy==LB_ROUND_ROBIN) { idx=l->rr_index; l->rr_index=(l->rr_index+1)%l->n; }
        else idx=tries;
        if (!l->e[idx].healthy) continue;
        if (l->strategy==LB_LEAST_CONN) { if (best<0||l->e[idx].active_conns<l->e[best].active_conns) best=idx; }
        else if (l->strategy==LB_LATENCY) { if (best<0||l->e[idx].latency_ms<l->e[best].latency_ms) best=idx; }
        else { best=idx; break; }
    }
    if (best>=0) l->e[best].active_conns++;
    return best;
}
void loadbalance_release(LoadBalance *l, int idx) { if(l&&idx>=0&&idx<l->n&&l->e[idx].active_conns>0) l->e[idx].active_conns--; }
int loadbalance_healthy_count(const LoadBalance *l) { if(!l) return -1; int n=0; for(int i=0;i<l->n;i++) if(l->e[i].healthy)n++; return n; }
