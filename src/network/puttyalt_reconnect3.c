/* puttyalt_reconnect3.c - Reconnect v3 with exponential backoff + jitter cap. */
#include <string.h>
#include <stdint.h>
typedef struct { int attempt; int max_attempts; int base_ms; int max_ms; uint32_t seed; int enabled; } Reconnect3;
void reconnect3_init(Reconnect3 *r, int max_attempts, int base_ms, int max_ms) {
    if(!r) return;
    memset(r,0,sizeof(*r)); r->max_attempts=max_attempts>0?max_attempts:10;
    r->base_ms=base_ms>0?base_ms:500; r->max_ms=max_ms>0?max_ms:60000; r->seed=0x9E3779B9; r->enabled=1;
}
int reconnect3_next_delay(Reconnect3 *r) {
    if(!r||!r->enabled||r->attempt>=r->max_attempts) return -1;
    int delay=r->base_ms;
    for (int i=0;i<r->attempt && delay<r->max_ms;i++) delay*=2;
    if (delay>r->max_ms) delay=r->max_ms;
    r->seed = r->seed*1664525u + 1013904223u;
    int jitter=(int)((r->seed>>16) % (delay/4 + 1));
    r->attempt++;
    return delay - delay/8 + jitter;
}
void reconnect3_success(Reconnect3 *r) { if(r) r->attempt=0; }
void reconnect3_disable(Reconnect3 *r) { if(r) r->enabled=0; }
int reconnect3_exhausted(const Reconnect3 *r) { return r? (r->attempt>=r->max_attempts):1; }
int reconnect3_attempt(const Reconnect3 *r) { return r?r->attempt:-1; }
