/* puttyalt_eventstream.c - Event stream with typed subscribers + replay buffer. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define ES_RING 128
#define ES_DATA 128
#define ES_SUBS 16
typedef struct { int type; char data[ES_DATA]; uint64_t seq; } es_event;
typedef struct { int type_mask; uint64_t cursor; int active; } es_sub;
typedef struct { es_event ring[ES_RING]; int head, count; uint64_t next_seq; es_sub subs[ES_SUBS]; int nsubs; } EventStream;
void eventstream_init(EventStream *e) { if(e){ memset(e,0,sizeof(*e)); e->next_seq=1; } }
uint64_t eventstream_publish(EventStream *e, int type, const char *data) {
    if(!e) return 0;
    int idx=(e->head+e->count)%ES_RING;
    if (e->count<ES_RING) e->count++; else e->head=(e->head+1)%ES_RING;
    es_event *ev=&e->ring[idx]; ev->type=type; snprintf(ev->data,ES_DATA,"%s",data?data:""); ev->seq=e->next_seq;
    return e->next_seq++;
}
int eventstream_subscribe(EventStream *e, int type_mask) {
    if(!e||e->nsubs>=ES_SUBS) return -1;
    es_sub *s=&e->subs[e->nsubs]; s->type_mask=type_mask; s->cursor=0; s->active=1; return e->nsubs++;
}
int eventstream_poll(EventStream *e, int sub_id, int *out_type, char *out_data, int datalen) {
    if(!e||sub_id<0||sub_id>=e->nsubs||!e->subs[sub_id].active) return -1;
    es_sub *s=&e->subs[sub_id];
    for (int i=0;i<e->count;i++) {
        int idx=(e->head+i)%ES_RING; es_event *ev=&e->ring[idx];
        if (ev->seq>s->cursor && (s->type_mask & (1<<ev->type))) {
            s->cursor=ev->seq;
            if (out_type) *out_type=ev->type;
            if (out_data) snprintf(out_data,datalen,"%s",ev->data);
            return 1;
        }
    }
    return 0;
}
int eventstream_pending(const EventStream *e, int sub_id) {
    if(!e||sub_id<0||sub_id>=e->nsubs) return -1;
    const es_sub *s=&e->subs[sub_id]; int n=0;
    for (int i=0;i<e->count;i++) { int idx=(e->head+i)%ES_RING; if (e->ring[idx].seq>s->cursor && (s->type_mask&(1<<e->ring[idx].type))) n++; }
    return n;
}
