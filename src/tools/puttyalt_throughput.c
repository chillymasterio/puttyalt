/* puttyalt_throughput.c - Throughput meter with EWMA smoothing + peak. */
#include <stdint.h>
typedef struct { uint64_t total_bytes; uint64_t window_bytes; uint64_t window_start_ms; double ewma_bps; double peak_bps; int alpha_pct; } Throughput;
void throughput_init(Throughput *t, int alpha_pct, uint64_t now_ms) {
    if(!t) return; t->total_bytes=0; t->window_bytes=0; t->window_start_ms=now_ms;
    t->ewma_bps=0; t->peak_bps=0; t->alpha_pct = (alpha_pct>0&&alpha_pct<=100)?alpha_pct:30;
}
void throughput_add(Throughput *t, uint64_t bytes) { if(t){ t->total_bytes+=bytes; t->window_bytes+=bytes; } }
int throughput_tick(Throughput *t, uint64_t now_ms) {
    if(!t) return -1;
    uint64_t dt = now_ms - t->window_start_ms;
    if (dt < 250) return 0; /* not enough time elapsed */
    double inst = (double)t->window_bytes * 1000.0 / dt;
    double a = t->alpha_pct / 100.0;
    t->ewma_bps = t->ewma_bps==0 ? inst : (a*inst + (1-a)*t->ewma_bps);
    if (t->ewma_bps > t->peak_bps) t->peak_bps = t->ewma_bps;
    t->window_bytes=0; t->window_start_ms=now_ms;
    return 1;
}
uint64_t throughput_bps(const Throughput *t) { return t?(uint64_t)t->ewma_bps:0; }
uint64_t throughput_peak(const Throughput *t) { return t?(uint64_t)t->peak_bps:0; }
uint64_t throughput_total(const Throughput *t) { return t?t->total_bytes:0; }
