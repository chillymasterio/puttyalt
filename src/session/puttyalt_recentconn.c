/* puttyalt_recentconn.c - Recent connections list with frequency + last-used. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define RC_MAX 32
#define RC_HOST 96
typedef struct { char host[RC_HOST]; int port; char user[48]; int count; uint64_t last_ms; } rc_entry;
typedef struct { rc_entry e[RC_MAX]; int n; } RecentConn;
void recentconn_init(RecentConn *r) { if(r) memset(r,0,sizeof(*r)); }
int recentconn_record(RecentConn *r, const char *host, int port, const char *user, uint64_t now_ms) {
    if(!r||!host) return -1;
    for (int i=0;i<r->n;i++) if (strcmp(r->e[i].host,host)==0 && r->e[i].port==port) {
        r->e[i].count++; r->e[i].last_ms=now_ms; return 0;
    }
    if (r->n>=RC_MAX) { /* evict least recently used */ int o=0; for(int i=1;i<r->n;i++) if(r->e[i].last_ms<r->e[o].last_ms)o=i;
        memmove(&r->e[o],&r->e[o+1],sizeof(rc_entry)*(r->n-o-1)); r->n--; }
    rc_entry *e=&r->e[r->n++]; snprintf(e->host,RC_HOST,"%s",host); e->port=port; snprintf(e->user,48,"%s",user?user:"");
    e->count=1; e->last_ms=now_ms; return 0;
}
int recentconn_top(const RecentConn *r, int by_freq, int *out_idx, int cap) {
    if(!r) return -1;
    int used[RC_MAX]={0}, n=0;
    for (int k=0;k<cap && k<r->n;k++) {
        int best=-1;
        for (int i=0;i<r->n;i++) if (!used[i]) {
            if (best<0) { best=i; continue; }
            long bi = by_freq ? r->e[i].count : (long)r->e[i].last_ms;
            long bb = by_freq ? r->e[best].count : (long)r->e[best].last_ms;
            if (bi>bb) best=i;
        }
        if (best<0) break;
        used[best]=1; out_idx[n++]=best;
    }
    return n;
}
int recentconn_count(const RecentConn *r) { return r?r->n:-1; }
