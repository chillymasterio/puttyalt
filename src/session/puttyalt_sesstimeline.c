/* puttyalt_sesstimeline.c - Session activity timeline with event markers. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define ST_MAX 128
#define ST_LABEL 48
enum st_kind { ST_CONNECT=0, ST_DISCONNECT=1, ST_COMMAND=2, ST_ERROR=3, ST_TRANSFER=4, ST_ALERT=5 };
typedef struct { uint64_t ts; int kind; char label[ST_LABEL]; } st_event;
typedef struct { st_event events[ST_MAX]; int n; } SessTimeline;
void sesstimeline_init(SessTimeline *t) { if(t) memset(t,0,sizeof(*t)); }
int sesstimeline_add(SessTimeline *t, uint64_t ts, int kind, const char *label) {
    if(!t) return -1;
    if (t->n>=ST_MAX) { memmove(&t->events[0],&t->events[1],sizeof(st_event)*(ST_MAX-1)); t->n--; }
    st_event *e=&t->events[t->n++]; e->ts=ts; e->kind=kind; snprintf(e->label,ST_LABEL,"%s",label?label:""); return 0;
}
int sesstimeline_in_range(const SessTimeline *t, uint64_t from, uint64_t to, int *out_idx, int cap) {
    if(!t) return -1; int n=0;
    for (int i=0;i<t->n && n<cap;i++) if (t->events[i].ts>=from && t->events[i].ts<=to) out_idx[n++]=i;
    return n;
}
int sesstimeline_count_kind(const SessTimeline *t, int kind) {
    if(!t) return -1; int n=0; for(int i=0;i<t->n;i++) if(t->events[i].kind==kind)n++; return n;
}
uint64_t sesstimeline_duration(const SessTimeline *t) {
    if(!t||t->n<2) return 0; return t->events[t->n-1].ts - t->events[0].ts;
}
int sesstimeline_count(const SessTimeline *t) { return t?t->n:-1; }
