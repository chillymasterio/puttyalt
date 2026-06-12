/* puttyalt_geoip.c - GeoIP region cache + latency-based region preference. */
#include <string.h>
#include <stdio.h>
#define GI_MAX 64
#define GI_IP 48
typedef struct { char ip_prefix[GI_IP]; char country[4]; char region[24]; int avg_latency; } gi_entry;
typedef struct { gi_entry e[GI_MAX]; int n; } GeoIp;
void geoip_init(GeoIp *g) { if(g) memset(g,0,sizeof(*g)); }
int geoip_add(GeoIp *g, const char *ip_prefix, const char *country, const char *region, int latency) {
    if(!g||g->n>=GI_MAX||!ip_prefix) return -1;
    gi_entry *e=&g->e[g->n++]; snprintf(e->ip_prefix,GI_IP,"%s",ip_prefix);
    snprintf(e->country,4,"%s",country?country:"??"); snprintf(e->region,24,"%s",region?region:"");
    e->avg_latency=latency; return 0;
}
int geoip_lookup(const GeoIp *g, const char *ip, char *country, int clen, char *region, int rlen) {
    if(!g||!ip) return -1;
    for (int i=0;i<g->n;i++) {
        int pl=(int)strlen(g->e[i].ip_prefix);
        if (strncmp(ip,g->e[i].ip_prefix,pl)==0) {
            if (country) snprintf(country,clen,"%s",g->e[i].country);
            if (region) snprintf(region,rlen,"%s",g->e[i].region);
            return 0;
        }
    }
    return -1;
}
int geoip_best_region(const GeoIp *g) {
    if(!g||g->n==0) return -1;
    int best=0;
    for (int i=1;i<g->n;i++) if (g->e[i].avg_latency<g->e[best].avg_latency) best=i;
    return best;
}
int geoip_count(const GeoIp *g) { return g?g->n:-1; }
