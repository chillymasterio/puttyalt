/* puttyalt_portscan.c - Port scan result aggregator (open/closed/filtered states). */
#include <string.h>
#include <stdio.h>
#define PS_MAX 64
enum ps_state { PS_CLOSED=0, PS_OPEN=1, PS_FILTERED=2 };
typedef struct { int port; int state; char service[24]; int rtt_ms; } ps_result;
typedef struct { ps_result r[PS_MAX]; int n; } PortScan;
void portscan_init(PortScan *p) { if(p) memset(p,0,sizeof(*p)); }
int portscan_record(PortScan *p, int port, int state, const char *service, int rtt_ms) {
    if(!p||p->n>=PS_MAX) return -1;
    ps_result *r=&p->r[p->n++]; r->port=port; r->state=state;
    snprintf(r->service,24,"%s",service?service:""); r->rtt_ms=rtt_ms; return 0;
}
int portscan_open_count(const PortScan *p) {
    if(!p) return -1; int n=0; for(int i=0;i<p->n;i++) if(p->r[i].state==PS_OPEN)n++; return n;
}
int portscan_summary(const PortScan *p, char *buf, int buflen) {
    if(!p||!buf) return -1; int pos=0;
    for (int i=0;i<p->n && pos<buflen;i++) if (p->r[i].state==PS_OPEN)
        pos+=snprintf(buf+pos,buflen-pos,"%d/%s open\n",p->r[i].port,p->r[i].service[0]?p->r[i].service:"?");
    return pos;
}
const char *portscan_guess_service(int port) {
    switch(port) { case 22: return "ssh"; case 80: return "http"; case 443: return "https";
        case 21: return "ftp"; case 3306: return "mysql"; case 5432: return "postgres";
        case 6379: return "redis"; case 27017: return "mongo"; case 3389: return "rdp"; default: return ""; }
}
int portscan_count(const PortScan *p) { return p?p->n:-1; }
