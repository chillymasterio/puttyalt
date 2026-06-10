/* puttyalt_ratecalc2.c - Transfer rate calculator with smoothed ETA. */
#include <stdint.h>
#define RC_SAMPLES 8
typedef struct { uint64_t samples[RC_SAMPLES]; uint64_t times[RC_SAMPLES]; int head, count; uint64_t total, target; } RateCalc2;
void ratecalc2_init(RateCalc2 *r, uint64_t target) {
    if(!r) return; for(int i=0;i<RC_SAMPLES;i++){r->samples[i]=0;r->times[i]=0;} r->head=0;r->count=0;r->total=0;r->target=target;
}
void ratecalc2_update(RateCalc2 *r, uint64_t cumulative, uint64_t now_ms) {
    if(!r) return; r->samples[r->head]=cumulative; r->times[r->head]=now_ms;
    r->head=(r->head+1)%RC_SAMPLES; if(r->count<RC_SAMPLES)r->count++; r->total=cumulative;
}
uint64_t ratecalc2_bps(const RateCalc2 *r) {
    if(!r||r->count<2) return 0;
    int oldest=(r->head-r->count+RC_SAMPLES)%RC_SAMPLES, newest=(r->head-1+RC_SAMPLES)%RC_SAMPLES;
    uint64_t db=r->samples[newest]-r->samples[oldest], dt=r->times[newest]-r->times[oldest];
    return dt? db*1000/dt : 0;
}
int64_t ratecalc2_eta_sec(const RateCalc2 *r) {
    if(!r||r->target==0) return -1; uint64_t bps=ratecalc2_bps(r); if(bps==0) return -1;
    uint64_t rem=r->target>r->total? r->target-r->total:0; return rem/bps;
}
int ratecalc2_percent(const RateCalc2 *r) { return (r&&r->target)? (int)(r->total*100/r->target):0; }
