/* puttyalt_counter2.c - Monotonic counter with rate computation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long value; long last_value; long last_time; double rate; } Counter2;
void ct5_init(Counter2 *c) { if (c) { c->value = 0; c->last_value = 0; c->last_time = 0; c->rate = 0; } }
void ct5_add(Counter2 *c, long delta) { if (c) c->value += delta; }
void ct5_inc(Counter2 *c) { if (c) c->value++; }
/* Compute per-second rate since last sample. */
double ct5_rate(Counter2 *c, long now) {
    if (!c) return 0;
    long dt = now - c->last_time;
    if (dt > 0) { c->rate = (double)(c->value - c->last_value) / dt; c->last_value = c->value; c->last_time = now; }
    return c->rate;
}
