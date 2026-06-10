/* puttyalt_connhistory2.c - Connection history v2 with success/failure stats. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CH_MAX 64
#define CH_HOST 96
typedef struct { char host[CH_HOST]; int port; int success_count; int fail_count; uint64_t last_success; uint64_t last_attempt; } ch_entry;
typedef struct { ch_entry entries[CH_MAX]; int n; } ConnHistory2;
void connhistory2_init(ConnHistory2 *c) { if(c) memset(c,0,sizeof(*c)); }
static ch_entry *ch_find(ConnHistory2 *c, const char *host, int port) {
    for(int i=0;i<c->n;i++) if(c->entries[i].port==port && strcmp(c->entries[i].host,host)==0) return &c->entries[i];
    if (c->n>=CH_MAX) return 0;
    ch_entry *e=&c->entries[c->n++]; snprintf(e->host,CH_HOST,"%s",host); e->port=port; return e;
}
int connhistory2_record(ConnHistory2 *c, const char *host, int port, int success, uint64_t now_ms) {
    if(!c||!host) return -1; ch_entry *e=ch_find(c,host,port); if(!e) return -1;
    e->last_attempt=now_ms;
    if (success) { e->success_count++; e->last_success=now_ms; } else e->fail_count++;
    return 0;
}
int connhistory2_reliability(const ConnHistory2 *c, const char *host, int port) {
    if(!c||!host) return -1;
    for (int i=0;i<c->n;i++) if (c->entries[i].port==port && strcmp(c->entries[i].host,host)==0) {
        int total=c->entries[i].success_count+c->entries[i].fail_count;
        return total? c->entries[i].success_count*100/total : -1;
    }
    return -1;
}
int connhistory2_count(const ConnHistory2 *c) { return c?c->n:-1; }
