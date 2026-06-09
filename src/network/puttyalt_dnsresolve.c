/* puttyalt_dnsresolve.c - DNS resolution result handler (A/AAAA/CNAME). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define DR_MAX 8
enum dr_type { DR_A=1, DR_AAAA=28, DR_CNAME=5 };
typedef struct { int type; char value[64]; int ttl; } dr_record;
typedef struct { char hostname[128]; dr_record records[DR_MAX]; int n; int resolved; } DnsResolve;
void dnsresolve_init(DnsResolve *d, const char *hostname) {
    if(!d) return; memset(d,0,sizeof(*d)); snprintf(d->hostname,128,"%s",hostname?hostname:"");
}
int dnsresolve_add(DnsResolve *d, int type, const char *value, int ttl) {
    if(!d||d->n>=DR_MAX||!value) return -1;
    dr_record *r=&d->records[d->n++]; r->type=type; snprintf(r->value,64,"%s",value); r->ttl=ttl;
    d->resolved=1; return 0;
}
const char *dnsresolve_first_ip(const DnsResolve *d, int prefer_v6) {
    if(!d) return 0;
    if (prefer_v6) for(int i=0;i<d->n;i++) if(d->records[i].type==DR_AAAA) return d->records[i].value;
    for (int i=0;i<d->n;i++) if (d->records[i].type==DR_A) return d->records[i].value;
    for (int i=0;i<d->n;i++) if (d->records[i].type==DR_AAAA) return d->records[i].value;
    return 0;
}
int dnsresolve_min_ttl(const DnsResolve *d) {
    if(!d||d->n==0) return 0; int m=d->records[0].ttl;
    for (int i=1;i<d->n;i++) if (d->records[i].ttl<m) m=d->records[i].ttl;
    return m;
}
int dnsresolve_count(const DnsResolve *d) { return d?d->n:-1; }
