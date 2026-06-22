/* puttyalt_latencystat.c - Track latency samples and jitter.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define LS_WIN 32
typedef struct { long samples[LS_WIN]; int n; int head; } LatencyStat;
void ls3_init(LatencyStat *s) { if (s) { s->n = 0; s->head = 0; } }
void ls3_add(LatencyStat *s, long ms) {
    if (!s) return;
    s->samples[s->head] = ms;
    s->head = (s->head + 1) % LS_WIN;
    if (s->n < LS_WIN) s->n++;
}
long ls3_avg(const LatencyStat *s) {
    if (!s || s->n == 0) return 0;
    long sum = 0;
    for (int i = 0; i < s->n; i++) sum += s->samples[i];
    return sum / s->n;
}
long ls3_max(const LatencyStat *s) {
    if (!s || s->n == 0) return 0;
    long m = s->samples[0];
    for (int i = 1; i < s->n; i++) if (s->samples[i] > m) m = s->samples[i];
    return m;
}
/* Jitter = mean absolute deviation of consecutive samples. */
long ls3_jitter(const LatencyStat *s) {
    if (!s || s->n < 2) return 0;
    long avg = ls3_avg(s), sum = 0;
    for (int i = 0; i < s->n; i++) { long d = s->samples[i] - avg; sum += d < 0 ? -d : d; }
    return sum / s->n;
}
