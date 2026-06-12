/* puttyalt_profiler.c - Lightweight function profiler (call counts + timing). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define PF_MAX 48
#define PF_NAME 40
typedef struct { char name[PF_NAME]; uint64_t call_count; uint64_t total_us; uint64_t max_us; uint64_t start_us; } perfprof_entry;
typedef struct { perfprof_entry entries[PF_MAX]; int n; } PerfProf;
void perfprof_init(PerfProf *p) { if(p) memset(p,0,sizeof(*p)); }
static perfprof_entry *perfprof_find(PerfProf *p, const char *name) {
    for(int i=0;i<p->n;i++) if(strcmp(p->entries[i].name,name)==0) return &p->entries[i];
    if (p->n>=PF_MAX) return 0;
    perfprof_entry *e=&p->entries[p->n++]; snprintf(e->name,PF_NAME,"%s",name); return e;
}
void perfprof_enter(PerfProf *p, const char *name, uint64_t now_us) {
    if(!p) return;
    perfprof_entry *e=perfprof_find(p,name); if(e) e->start_us=now_us;
}
void perfprof_exit(PerfProf *p, const char *name, uint64_t now_us) {
    if(!p) return;
    perfprof_entry *e=perfprof_find(p,name); if(!e) return;
    uint64_t dur=now_us-e->start_us; e->call_count++; e->total_us+=dur;
    if (dur>e->max_us) e->max_us=dur;
}
uint64_t perfprof_avg_us(const PerfProf *p, const char *name) {
    if(!p||!name) return 0;
    for (int i=0;i<p->n;i++) if (strcmp(p->entries[i].name,name)==0)
        return p->entries[i].call_count? p->entries[i].total_us/p->entries[i].call_count : 0;
    return 0;
}
int perfprof_report(const PerfProf *p, char *out, int outlen) {
    if(!p||!out) return -1;
    int pos=0;
    for (int i=0;i<p->n && pos<outlen;i++)
        pos+=snprintf(out+pos,outlen-pos,"%s: %llu calls, avg %lluus, max %lluus\n",
            p->entries[i].name,(unsigned long long)p->entries[i].call_count,
            (unsigned long long)(p->entries[i].call_count?p->entries[i].total_us/p->entries[i].call_count:0),
            (unsigned long long)p->entries[i].max_us);
    return pos;
}
int perfprof_count(const PerfProf *p) { return p?p->n:-1; }
