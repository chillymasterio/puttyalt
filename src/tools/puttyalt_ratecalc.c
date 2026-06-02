/* puttyalt_ratecalc.c - Sliding-window transfer rate + ETA calculator. */
#include <stdint.h>
#define RC_SAMPLES 16
typedef struct { uint64_t bytes[RC_SAMPLES]; uint64_t ts[RC_SAMPLES]; int head, count; uint64_t total_bytes; uint64_t total_target; } RateCalc;
void ratecalc_init(RateCalc *r, uint64_t target) { if(r){ for(int i=0;i<RC_SAMPLES;i++){r->bytes[i]=0;r->ts[i]=0;} r->head=0;r->count=0;r->total_bytes=0;r->total_target=target; } }
void ratecalc_sample(RateCalc *r, uint64_t cumulative_bytes, uint64_t now_ms) {
    if(!r) return;
    r->bytes[r->head]=cumulative_bytes; r->ts[r->head]=now_ms;
    r->head=(r->head+1)%RC_SAMPLES; if(r->count<RC_SAMPLES) r->count++;
    r->total_bytes=cumulative_bytes;
}
uint64_t ratecalc_bps(const RateCalc *r) {
    if(!r||r->count<2) return 0;
    int oldest=(r->head - r->count + RC_SAMPLES)%RC_SAMPLES;
    int newest=(r->head - 1 + RC_SAMPLES)%RC_SAMPLES;
    uint64_t db=r->bytes[newest]-r->bytes[oldest];
    uint64_t dt=r->ts[newest]-r->ts[oldest];
    return dt? (db*1000)/dt : 0;
}
int64_t ratecalc_eta_ms(const RateCalc *r) {
    if(!r||r->total_target==0) return -1;
    uint64_t bps=ratecalc_bps(r); if(bps==0) return -1;
    uint64_t remain = r->total_target>r->total_bytes ? r->total_target-r->total_bytes : 0;
    return (int64_t)((remain*1000)/bps);
}
int ratecalc_pct(const RateCalc *r) { return (r&&r->total_target)? (int)((r->total_bytes*100)/r->total_target):0; }
