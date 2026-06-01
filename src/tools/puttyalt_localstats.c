/* puttyalt_localstats.c - Privacy-first local usage analytics (no network, ring of events). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define LS_MAX_COUNTERS 48
#define LS_NAME 32
typedef struct { char name[LS_NAME]; uint64_t count; uint64_t last_ms; } ls_counter;
typedef struct { ls_counter c[LS_MAX_COUNTERS]; int n; uint64_t session_start_ms; } LocalStats;
void localstats_init(LocalStats *s, uint64_t now_ms) {
    if(!s) return; memset(s,0,sizeof(*s)); s->session_start_ms=now_ms;
}
int localstats_incr(LocalStats *s, const char *name, uint64_t now_ms) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->c[i].name,name)==0) { s->c[i].count++; s->c[i].last_ms=now_ms; return 0; }
    if (s->n>=LS_MAX_COUNTERS) return -1;
    ls_counter *c=&s->c[s->n++]; snprintf(c->name,LS_NAME,"%s",name); c->count=1; c->last_ms=now_ms; return 0;
}
uint64_t localstats_get(const LocalStats *s, const char *name) {
    if(!s||!name) return 0;
    for (int i=0;i<s->n;i++) if (strcmp(s->c[i].name,name)==0) return s->c[i].count;
    return 0;
}
int localstats_export(const LocalStats *s, char *buf, int buflen, uint64_t now_ms) {
    if(!s||!buf) return -1;
    int pos=snprintf(buf,buflen,"uptime_s=%llu\n",(unsigned long long)((now_ms-s->session_start_ms)/1000));
    for (int i=0;i<s->n && pos<buflen;i++)
        pos+=snprintf(buf+pos,buflen-pos,"%s=%llu\n",s->c[i].name,(unsigned long long)s->c[i].count);
    return pos;
}
void localstats_reset(LocalStats *s, uint64_t now_ms) { if(s) localstats_init(s,now_ms); }
int localstats_counter_count(const LocalStats *s) { return s?s->n:-1; }
