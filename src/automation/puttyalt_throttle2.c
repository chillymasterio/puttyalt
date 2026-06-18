/* puttyalt_throttle2.c - Throttle action to N per time window.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define TH_WIN 32
typedef struct { long times[TH_WIN]; int n; int limit; long window; } Throttle2;
void th2_init(Throttle2 *t, int limit, long window) {
    if (!t) return;
    t->n = 0; t->limit = (limit > 0 && limit <= TH_WIN) ? limit : TH_WIN; t->window = window > 0 ? window : 1;
}
/* Returns 1 if allowed at `now`, recording the event. */
int th2_allow(Throttle2 *t, long now) {
    if (!t) return 0;
    /* drop expired */
    int w = 0;
    for (int i = 0; i < t->n; i++) if (now - t->times[i] < t->window) t->times[w++] = t->times[i];
    t->n = w;
    if (t->n >= t->limit) return 0;
    if (t->n < TH_WIN) t->times[t->n++] = now;
    return 1;
}
int th2_count(const Throttle2 *t) { return t ? t->n : 0; }
