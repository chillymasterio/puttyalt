/* puttyalt_keepalivecalc.c - Compute adaptive keepalive intervals.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long base; long current; long max; int idle_count; } KeepaliveCalc;
void kac_init(KeepaliveCalc *k, long base, long max) {
    if (!k) return;
    k->base = base > 0 ? base : 30;
    k->max = max > 0 ? max : 300;
    k->current = k->base; k->idle_count = 0;
}
/* Backoff when idle, reset on activity. Returns next interval. */
long kac_on_idle(KeepaliveCalc *k) {
    if (!k) return 0;
    k->idle_count++;
    k->current = k->current * 3 / 2;
    if (k->current > k->max) k->current = k->max;
    return k->current;
}
void kac_on_activity(KeepaliveCalc *k) {
    if (!k) return;
    k->current = k->base; k->idle_count = 0;
}
