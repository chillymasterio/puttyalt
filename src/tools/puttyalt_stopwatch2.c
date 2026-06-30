/* puttyalt_stopwatch2.c - Stopwatch with lap timing.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define SW_LAPS 16
typedef struct { long start; long laps[SW_LAPS]; int nlaps; int running; } Stopwatch2;
void sw4_start(Stopwatch2 *s, long now) { if (s) { s->start = now; s->nlaps = 0; s->running = 1; } }
long sw4_lap(Stopwatch2 *s, long now) {
    if (!s || !s->running) return -1;
    long elapsed = now - s->start;
    if (s->nlaps < SW_LAPS) s->laps[s->nlaps++] = elapsed;
    return elapsed;
}
long sw4_stop(Stopwatch2 *s, long now) {
    if (!s || !s->running) return -1;
    s->running = 0;
    return now - s->start;
}
long sw4_lap_delta(const Stopwatch2 *s, int i) {
    if (!s || i <= 0 || i >= s->nlaps) return i == 0 && s->nlaps > 0 ? s->laps[0] : -1;
    return s->laps[i] - s->laps[i-1];
}
