/* puttyalt_portforward2.c - Port forwarding manager v2 (local/remote/dynamic). */
#include <string.h>
#include <stdio.h>
#define PF_MAX 32
#define PF_HOST 96
enum pf_type { PF_LOCAL=0, PF_REMOTE=1, PF_DYNAMIC=2 };
typedef struct { int type; int listen_port; char dest_host[PF_HOST]; int dest_port; int active; int conns; } pf_rule;
typedef struct { pf_rule r[PF_MAX]; int n; } PortForward2;
void portforward2_init(PortForward2 *p) { if(p) memset(p,0,sizeof(*p)); }
int portforward2_add(PortForward2 *p, int type, int listen_port, const char *dest_host, int dest_port) {
    if(!p||p->n>=PF_MAX) return -1;
    pf_rule *r=&p->r[p->n]; r->type=type; r->listen_port=listen_port;
    snprintf(r->dest_host,PF_HOST,"%s",dest_host?dest_host:""); r->dest_port=dest_port; r->active=1;
    return p->n++;
}
int portforward2_find_by_port(const PortForward2 *p, int listen_port) {
    if(!p) return -1;
    for (int i=0;i<p->n;i++) if (p->r[i].listen_port==listen_port && p->r[i].active) return i;
    return -1;
}
void portforward2_track_conn(PortForward2 *p, int idx, int delta) {
    if(!p||idx<0||idx>=p->n) return; p->r[idx].conns+=delta; if(p->r[idx].conns<0) p->r[idx].conns=0;
}
int portforward2_format(const PortForward2 *p, int idx, char *out, int outlen) {
    if(!p||idx<0||idx>=p->n||!out) return -1;
    const pf_rule *r=&p->r[idx];
    static const char *tn[]={"L","R","D"};
    if (r->type==PF_DYNAMIC) return snprintf(out,outlen,"-D %d (SOCKS)",r->listen_port);
    return snprintf(out,outlen,"-%s %d:%s:%d",tn[r->type],r->listen_port,r->dest_host,r->dest_port);
}
int portforward2_active_count(const PortForward2 *p) {
    if(!p) return -1; int n=0; for(int i=0;i<p->n;i++) if(p->r[i].active)n++; return n;
}
