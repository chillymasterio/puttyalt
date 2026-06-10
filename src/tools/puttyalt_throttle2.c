/* puttyalt_throttle2.c - Output throttle (prevent terminal flooding). */
#include <stdint.h>
typedef struct { uint64_t bytes_window; uint64_t window_start_ms; int max_bytes_per_sec; int throttled; uint64_t dropped; } Throttle2;
void throttle2_init(Throttle2 *t, int max_bytes_per_sec, uint64_t now_ms) {
    if(!t) return; t->bytes_window=0; t->window_start_ms=now_ms; t->max_bytes_per_sec=max_bytes_per_sec; t->throttled=0; t->dropped=0;
}
int throttle2_allow(Throttle2 *t, int bytes, uint64_t now_ms) {
    if(!t||t->max_bytes_per_sec<=0) return bytes; /* unlimited */
    if (now_ms-t->window_start_ms>=1000) { t->bytes_window=0; t->window_start_ms=now_ms; t->throttled=0; }
    if (t->bytes_window+bytes > (uint64_t)t->max_bytes_per_sec) {
        int allowed=(int)((uint64_t)t->max_bytes_per_sec - t->bytes_window);
        if (allowed<0) allowed=0;
        t->bytes_window+=allowed; t->throttled=1; t->dropped+=(bytes-allowed);
        return allowed;
    }
    t->bytes_window+=bytes; return bytes;
}
int throttle2_is_throttled(const Throttle2 *t) { return t?t->throttled:-1; }
uint64_t throttle2_dropped(const Throttle2 *t) { return t?t->dropped:0; }
