/* puttyalt_bandwidthlimit2.c - Bandwidth limiter v2 with burst allowance. */
#include <stdint.h>
typedef struct { int64_t rate_bps; int64_t burst_bytes; double tokens; uint64_t last_ms; uint64_t throttled_count; } BandwidthLimit2;
void bandwidthlimit2_init(BandwidthLimit2 *b, int64_t rate_bps, int64_t burst, uint64_t now_ms) {
    if(!b) return;
    b->rate_bps=rate_bps; b->burst_bytes=burst>0?burst:rate_bps; b->tokens=b->burst_bytes; b->last_ms=now_ms; b->throttled_count=0;
}
int64_t bandwidthlimit2_request(BandwidthLimit2 *b, int64_t bytes, uint64_t now_ms) {
    if(!b||b->rate_bps<=0) return bytes;
    uint64_t dt=now_ms-b->last_ms; b->last_ms=now_ms;
    b->tokens += (double)b->rate_bps*dt/1000.0;
    if (b->tokens>b->burst_bytes) b->tokens=b->burst_bytes;
    if (b->tokens>=bytes) { b->tokens-=bytes; return bytes; }
    int64_t allowed=(int64_t)b->tokens; b->tokens-=allowed; b->throttled_count++;
    return allowed;
}
int64_t bandwidthlimit2_available(const BandwidthLimit2 *b) { return b?(int64_t)b->tokens:0; }
uint64_t bandwidthlimit2_throttled(const BandwidthLimit2 *b) { return b?b->throttled_count:0; }
