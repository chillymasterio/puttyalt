/* puttyalt_proxyhop.c - Multi-hop proxy chain builder (SOCKS/HTTP/SSH hops). */
#include <string.h>
#include <stdio.h>
#define PH_MAX 8
#define PH_HOST 96
enum ph_type { PH_SOCKS5=0, PH_HTTP=1, PH_SSH=2 };
typedef struct { char host[PH_HOST]; int port; int type; } ph_hop;
typedef struct { ph_hop hops[PH_MAX]; int n; } ProxyHop;
void proxyhop_init(ProxyHop *p) { if(p) memset(p,0,sizeof(*p)); }
int proxyhop_add(ProxyHop *p, const char *host, int port, int type) {
    if(!p||p->n>=PH_MAX||!host) return -1;
    ph_hop *h=&p->hops[p->n++]; snprintf(h->host,PH_HOST,"%s",host); h->port=port; h->type=type; return 0;
}
int proxyhop_describe(const ProxyHop *p, char *out, int outlen) {
    if(!p||!out) return -1;
    int pos=0;
    static const char *tn[]={"socks5","http","ssh"};
    for (int i=0;i<p->n && pos<outlen;i++)
        pos+=snprintf(out+pos,outlen-pos,"%s%s://%s:%d",i?" -> ":"",p->hops[i].type<=PH_SSH?tn[p->hops[i].type]:"?",p->hops[i].host,p->hops[i].port);
    return pos;
}
int proxyhop_count(const ProxyHop *p) { return p?p->n:-1; }
int proxyhop_get(const ProxyHop *p, int idx, char *host, int hostlen, int *port, int *type) {
    if(!p||idx<0||idx>=p->n) return -1;
    if (host) snprintf(host,hostlen,"%s",p->hops[idx].host);
    if (port) *port=p->hops[idx].port; if (type) *type=p->hops[idx].type; return 0;
}
