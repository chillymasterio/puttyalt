/* puttyalt_connpool3.c - Connection pool v3 with idle timeout + warmup. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CP_MAX 16
#define CP_KEY 96
enum cp_st { CP_FREE=0, CP_INUSE=1, CP_WARM=2 };
typedef struct { char key[CP_KEY]; int state; uint64_t idle_since; int uses; } cp_conn;
typedef struct { cp_conn conns[CP_MAX]; int n; int idle_timeout_ms; int min_warm; } ConnPool3;
void connpool3_init(ConnPool3 *p, int idle_timeout_ms, int min_warm) {
    if(!p) return; memset(p,0,sizeof(*p)); p->idle_timeout_ms=idle_timeout_ms>0?idle_timeout_ms:60000; p->min_warm=min_warm;
}
int connpool3_acquire(ConnPool3 *p, const char *key, uint64_t now_ms) {
    if(!p||!key) return -1;
    for (int i=0;i<p->n;i++) if (p->conns[i].state!=CP_INUSE && strcmp(p->conns[i].key,key)==0) {
        p->conns[i].state=CP_INUSE; p->conns[i].uses++; return i;
    }
    if (p->n>=CP_MAX) return -1;
    cp_conn *c=&p->conns[p->n]; snprintf(c->key,CP_KEY,"%s",key); c->state=CP_INUSE; c->uses=1; return p->n++;
}
void connpool3_release(ConnPool3 *p, int idx, uint64_t now_ms) {
    if(!p||idx<0||idx>=p->n) return; p->conns[idx].state=CP_FREE; p->conns[idx].idle_since=now_ms;
}
int connpool3_reap(ConnPool3 *p, uint64_t now_ms) {
    if(!p) return -1; int reaped=0; int warm=0;
    for (int i=0;i<p->n;i++) if (p->conns[i].state==CP_WARM) warm++;
    for (int i=0;i<p->n;i++) if (p->conns[i].state==CP_FREE && (now_ms-p->conns[i].idle_since)>(uint64_t)p->idle_timeout_ms) {
        if (warm>=p->min_warm) { memmove(&p->conns[i],&p->conns[i+1],sizeof(cp_conn)*(p->n-i-1)); p->n--; i--; reaped++; }
        else { p->conns[i].state=CP_WARM; warm++; }
    }
    return reaped;
}
int connpool3_inuse(const ConnPool3 *p) { if(!p) return -1; int n=0; for(int i=0;i<p->n;i++) if(p->conns[i].state==CP_INUSE)n++; return n; }
int connpool3_count(const ConnPool3 *p) { return p?p->n:-1; }
