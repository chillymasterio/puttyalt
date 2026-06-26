/* puttyalt_apiratelimit.c - Track API rate-limit headers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int limit; int remaining; long reset_at; } ApiRateLimit;
void ar2_init(ApiRateLimit *r) { if (r) { r->limit = 0; r->remaining = -1; r->reset_at = 0; } }
void ar2_update(ApiRateLimit *r, int limit, int remaining, long reset_at) {
    if (!r) return;
    r->limit = limit; r->remaining = remaining; r->reset_at = reset_at;
}
/* Should we wait before making another request? */
int ar2_should_wait(const ApiRateLimit *r, long now) {
    if (!r || r->remaining < 0) return 0;
    return r->remaining == 0 && now < r->reset_at;
}
long ar2_wait_seconds(const ApiRateLimit *r, long now) {
    if (!ar2_should_wait(r, now)) return 0;
    return r->reset_at - now;
}
int ar2_pct_used(const ApiRateLimit *r) {
    if (!r || r->limit == 0) return 0;
    return (r->limit - r->remaining) * 100 / r->limit;
}
