/* puttyalt_dohresolve.c - DNS-over-HTTPS resolver cache + record TTL handling. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define DH_MAX 64
#define DH_HOST 96
#define DH_ADDR 48
typedef struct { char host[DH_HOST]; char addr[DH_ADDR]; uint64_t expires_ms; int family; } dh_record;
typedef struct { dh_record r[DH_MAX]; int n; int hits, misses; } DohResolve;
void dohresolve_init(DohResolve *d) { if(d) memset(d,0,sizeof(*d)); }
int dohresolve_cache(DohResolve *d, const char *host, const char *addr, int family, int ttl_sec, uint64_t now_ms) {
    if(!d||!host||!addr) return -1;
    for (int i=0;i<d->n;i++) if (strcmp(d->r[i].host,host)==0 && d->r[i].family==family) {
        snprintf(d->r[i].addr,DH_ADDR,"%s",addr); d->r[i].expires_ms=now_ms+(uint64_t)ttl_sec*1000; return 0;
    }
    if (d->n>=DH_MAX) { /* evict expired or oldest */ int o=0; for(int i=1;i<d->n;i++) if(d->r[i].expires_ms<d->r[o].expires_ms)o=i; d->r[o]=d->r[d->n-1]; d->n--; }
    dh_record *r=&d->r[d->n++]; snprintf(r->host,DH_HOST,"%s",host); snprintf(r->addr,DH_ADDR,"%s",addr);
    r->family=family; r->expires_ms=now_ms+(uint64_t)ttl_sec*1000; return 0;
}
int dohresolve_lookup(DohResolve *d, const char *host, int family, uint64_t now_ms, char *out, int outlen) {
    if(!d||!host) return -1;
    for (int i=0;i<d->n;i++) if (strcmp(d->r[i].host,host)==0 && d->r[i].family==family) {
        if (now_ms>=d->r[i].expires_ms) { d->misses++; return -1; }
        d->hits++; if(out) snprintf(out,outlen,"%s",d->r[i].addr); return 0;
    }
    d->misses++; return -1;
}
int dohresolve_purge_expired(DohResolve *d, uint64_t now_ms) {
    if(!d) return -1; int p=0;
    for (int i=0;i<d->n;i++) if (now_ms>=d->r[i].expires_ms) { memmove(&d->r[i],&d->r[i+1],sizeof(dh_record)*(d->n-i-1)); d->n--; i--; p++; }
    return p;
}
int dohresolve_hit_rate(const DohResolve *d) { if(!d) return -1; int t=d->hits+d->misses; return t?(d->hits*100)/t:0; }
