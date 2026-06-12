/* puttyalt_connpool2.c - Connection pool v2 with health-based reuse + warmup. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CP2_MAX 32
#define CP2_HOST 96
enum cp2_state { CP2_IDLE=0, CP2_BUSY=1, CP2_UNHEALTHY=2, CP2_WARMING=3 };
typedef struct { char host[CP2_HOST]; int port; int state; int health; uint64_t last_used; int reuse_count; } cp2_conn;
typedef struct { cp2_conn c[CP2_MAX]; int n; int max_idle_ms; } ConnPool2;
void connpool2_init(ConnPool2 *p, int max_idle_ms) {
    if(!p) return;
    memset(p,0,sizeof(*p)); p->max_idle_ms=max_idle_ms>0?max_idle_ms:300000;
}
int connpool2_acquire(ConnPool2 *p, const char *host, int port, uint64_t now_ms) {
    if(!p||!host) return -1;
    /* reuse healthiest idle conn to same host */
    int best=-1, bh=-1;
    for (int i=0;i<p->n;i++) if (p->c[i].state==CP2_IDLE && strcmp(p->c[i].host,host)==0 && p->c[i].port==port && p->c[i].health>bh) { bh=p->c[i].health; best=i; }
    if (best>=0) { p->c[best].state=CP2_BUSY; p->c[best].last_used=now_ms; p->c[best].reuse_count++; return best; }
    if (p->n>=CP2_MAX) return -1;
    cp2_conn *c=&p->c[p->n]; snprintf(c->host,CP2_HOST,"%s",host); c->port=port; c->state=CP2_WARMING; c->health=100; c->last_used=now_ms;
    return p->n++;
}
void connpool2_release(ConnPool2 *p, int idx, int healthy) {
    if(!p||idx<0||idx>=p->n) return;
    p->c[idx].state = healthy?CP2_IDLE:CP2_UNHEALTHY;
    if (!healthy && p->c[idx].health>0) p->c[idx].health-=20;
}
int connpool2_reap_idle(ConnPool2 *p, uint64_t now_ms) {
    if(!p) return -1;
    int reaped=0;
    for (int i=0;i<p->n;i++) if ((p->c[i].state==CP2_IDLE||p->c[i].state==CP2_UNHEALTHY) && (now_ms-p->c[i].last_used)>(uint64_t)p->max_idle_ms) {
        memmove(&p->c[i],&p->c[i+1],sizeof(cp2_conn)*(p->n-i-1)); p->n--; i--; reaped++;
    }
    return reaped;
}
int connpool2_active(const ConnPool2 *p) { if(!p) return -1; int n=0; for(int i=0;i<p->n;i++) if(p->c[i].state==CP2_BUSY)n++; return n; }
int connpool2_size(const ConnPool2 *p) { return p?p->n:-1; }
