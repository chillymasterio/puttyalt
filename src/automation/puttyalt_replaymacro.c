/* puttyalt_replaymacro.c - Recorded session replay with speed control. */
#include <string.h>
#include <stdint.h>
#define RM_MAX 256
typedef struct { uint64_t offset_ms; int data_len; } rm_frame;
typedef struct { rm_frame frames[RM_MAX]; int n; int cursor; int speed_pct; uint64_t base_ms; int playing; } ReplayMacro;
void replaymacro_init(ReplayMacro *r) { if(r){ memset(r,0,sizeof(*r)); r->speed_pct=100; } }
int replaymacro_add(ReplayMacro *r, uint64_t offset_ms, int data_len) {
    if(!r||r->n>=RM_MAX) return -1;
    r->frames[r->n].offset_ms=offset_ms; r->frames[r->n].data_len=data_len; return r->n++;
}
void replaymacro_set_speed(ReplayMacro *r, int pct) { if(r) r->speed_pct=pct>0?pct:100; }
int replaymacro_start(ReplayMacro *r, uint64_t now_ms) {
    if(!r||r->n==0) return -1;
    r->cursor=0; r->base_ms=now_ms; r->playing=1; return 0;
}
int replaymacro_due(ReplayMacro *r, uint64_t now_ms) {
    if(!r||!r->playing||r->cursor>=r->n) return -1;
    uint64_t elapsed=(now_ms-r->base_ms)*r->speed_pct/100;
    if (elapsed>=r->frames[r->cursor].offset_ms) return r->cursor;
    return -1;
}
int replaymacro_advance(ReplayMacro *r) {
    if(!r||r->cursor>=r->n) return -1;
    r->cursor++;
    if (r->cursor>=r->n) r->playing=0;
    return r->cursor;
}
int replaymacro_progress(const ReplayMacro *r) { return (r&&r->n)? r->cursor*100/r->n:0; }
