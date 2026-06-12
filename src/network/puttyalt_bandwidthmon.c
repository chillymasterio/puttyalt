/* puttyalt_bandwidthmon.c - Bandwidth monitor with per-session aggregation. */
#include <string.h>
#include <stdint.h>
#define BM_SESSIONS 16
typedef struct { int session_id; uint64_t bytes_in, bytes_out; uint64_t window_in, window_out; } bm_session;
typedef struct { bm_session sessions[BM_SESSIONS]; int n; uint64_t window_start_ms; } BandwidthMon;
void bandwidthmon_init(BandwidthMon *b, uint64_t now_ms) {
    if(!b) return;
    memset(b,0,sizeof(*b)); b->window_start_ms=now_ms;
}
static bm_session *bm_find(BandwidthMon *b, int session_id) {
    for(int i=0;i<b->n;i++) if(b->sessions[i].session_id==session_id) return &b->sessions[i];
    if (b->n>=BM_SESSIONS) return 0;
    bm_session *s=&b->sessions[b->n++]; s->session_id=session_id; return s;
}
void bandwidthmon_record(BandwidthMon *b, int session_id, uint64_t in_bytes, uint64_t out_bytes) {
    if(!b) return;
    bm_session *s=bm_find(b,session_id); if(!s) return;
    s->bytes_in+=in_bytes; s->bytes_out+=out_bytes; s->window_in+=in_bytes; s->window_out+=out_bytes;
}
uint64_t bandwidthmon_total_in(const BandwidthMon *b) {
    if(!b) return 0;
    uint64_t t=0; for(int i=0;i<b->n;i++) t+=b->sessions[i].bytes_in; return t;
}
uint64_t bandwidthmon_rate_in(BandwidthMon *b, int session_id, uint64_t now_ms) {
    if(!b) return 0;
    bm_session *s=bm_find(b,session_id); if(!s) return 0;
    uint64_t dt=now_ms-b->window_start_ms; if(dt==0) return 0;
    return s->window_in*1000/dt;
}
void bandwidthmon_reset_window(BandwidthMon *b, uint64_t now_ms) {
    if(!b) return;
    for(int i=0;i<b->n;i++){ b->sessions[i].window_in=0; b->sessions[i].window_out=0; } b->window_start_ms=now_ms;
}
int bandwidthmon_session_count(const BandwidthMon *b) { return b?b->n:-1; }
