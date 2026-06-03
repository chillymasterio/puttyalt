/* puttyalt_tracer.c - Lightweight span tracer (start/end + parent links). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TR_MAX 64
#define TR_NAME 48
typedef struct { char name[TR_NAME]; uint64_t start_ms; uint64_t end_ms; int parent; int id; int open; } tr_span;
typedef struct { tr_span s[TR_MAX]; int n; int next_id; int active; } Tracer;
void tracer_init(Tracer *t) { if(t){ memset(t,0,sizeof(*t)); t->next_id=1; t->active=-1; } }
int tracer_begin(Tracer *t, const char *name, uint64_t now_ms) {
    if(!t||t->n>=TR_MAX) return -1;
    tr_span *s=&t->s[t->n]; snprintf(s->name,TR_NAME,"%s",name?name:""); s->start_ms=now_ms;
    s->parent=t->active; s->id=t->next_id++; s->open=1; t->active=t->n; return t->n++;
}
int tracer_end(Tracer *t, int span_idx, uint64_t now_ms) {
    if(!t||span_idx<0||span_idx>=t->n||!t->s[span_idx].open) return -1;
    t->s[span_idx].end_ms=now_ms; t->s[span_idx].open=0;
    t->active = t->s[span_idx].parent; return 0;
}
uint64_t tracer_duration(const Tracer *t, int span_idx) {
    if(!t||span_idx<0||span_idx>=t->n) return 0;
    if (t->s[span_idx].open) return 0;
    return t->s[span_idx].end_ms - t->s[span_idx].start_ms;
}
int tracer_span_count(const Tracer *t) { return t?t->n:-1; }
int tracer_open_spans(const Tracer *t) { if(!t) return -1; int n=0; for(int i=0;i<t->n;i++) if(t->s[i].open)n++; return n; }
