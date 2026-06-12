/* puttyalt_ratelimit2.c - Sliding-window rate limiter for automation actions. */
#include <stdint.h>
#define RL_WINDOW 32
typedef struct { uint64_t timestamps[RL_WINDOW]; int head, count; int limit; int window_ms; int rejected; } RateLimit2;
void ratelimit2_init(RateLimit2 *r, int limit, int window_ms) {
    if(!r) return;
    for(int i=0;i<RL_WINDOW;i++) r->timestamps[i]=0;
    r->head=0; r->count=0; r->limit=limit>0&&limit<=RL_WINDOW?limit:RL_WINDOW; r->window_ms=window_ms>0?window_ms:1000; r->rejected=0;
}
static void rl_evict(RateLimit2 *r, uint64_t now_ms) {
    while (r->count>0) {
        int oldest=(r->head - r->count + RL_WINDOW)%RL_WINDOW;
        if ((now_ms - r->timestamps[oldest]) > (uint64_t)r->window_ms) r->count--;
        else break;
    }
}
int ratelimit2_allow(RateLimit2 *r, uint64_t now_ms) {
    if(!r) return 0;
    rl_evict(r,now_ms);
    if (r->count>=r->limit) { r->rejected++; return 0; }
    r->timestamps[r->head]=now_ms; r->head=(r->head+1)%RL_WINDOW; r->count++; return 1;
}
int ratelimit2_current(const RateLimit2 *r) { return r?r->count:-1; }
int ratelimit2_rejected(const RateLimit2 *r) { return r?r->rejected:-1; }
