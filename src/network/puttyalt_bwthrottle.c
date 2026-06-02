/* puttyalt_bwthrottle.c - Token-bucket bandwidth throttle for transfers. */
#include <stdint.h>
typedef struct { int64_t rate_bps; int64_t capacity; double tokens; uint64_t last_ms; } BwThrottle;
void bwthrottle_init(BwThrottle *t, int64_t rate_bps, uint64_t now_ms) {
    if(!t) return; t->rate_bps=rate_bps>0?rate_bps:0; t->capacity=rate_bps>0?rate_bps:0;
    t->tokens=t->capacity; t->last_ms=now_ms;
}
void bwthrottle_refill(BwThrottle *t, uint64_t now_ms) {
    if(!t||t->rate_bps<=0) return;
    uint64_t dt=now_ms-t->last_ms; t->last_ms=now_ms;
    t->tokens += ((double)t->rate_bps * dt)/1000.0;
    if (t->tokens>t->capacity) t->tokens=t->capacity;
}
int bwthrottle_allow(BwThrottle *t, int bytes, uint64_t now_ms) {
    if(!t||t->rate_bps<=0) return bytes; /* unlimited */
    bwthrottle_refill(t,now_ms);
    if (t->tokens>=bytes) { t->tokens-=bytes; return bytes; }
    int avail=(int)t->tokens; t->tokens-=avail; return avail;
}
int64_t bwthrottle_rate(const BwThrottle *t) { return t?t->rate_bps:0; }
