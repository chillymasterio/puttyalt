/* puttyalt_blinkstate.c - Track per-cell blink attribute timing.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long period_ms; long last_toggle; int phase; } BlinkState;
void bs3_init(BlinkState *b, long period_ms) {
    if (!b) return;
    b->period_ms = period_ms > 0 ? period_ms : 500;
    b->last_toggle = 0; b->phase = 1;
}
/* Update phase; returns 1 if cells with blink attr should be drawn. */
int bs3_tick(BlinkState *b, long now_ms) {
    if (!b) return 1;
    if (now_ms - b->last_toggle >= b->period_ms) { b->phase = !b->phase; b->last_toggle = now_ms; }
    return b->phase;
}
int bs3_visible(const BlinkState *b) { return b ? b->phase : 1; }
