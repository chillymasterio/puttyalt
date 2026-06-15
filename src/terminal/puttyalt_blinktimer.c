/* puttyalt_blinktimer.c - Cursor blink phase tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long period_ms; long last_toggle; int visible; int enabled; } BlinkTimer;
void blt_init(BlinkTimer *b, long period_ms) {
    if (!b) return;
    b->period_ms = period_ms > 0 ? period_ms : 530;
    b->last_toggle = 0; b->visible = 1; b->enabled = 1;
}
/* Update phase given current time; returns 1 if visibility changed. */
int blt_tick(BlinkTimer *b, long now_ms) {
    if (!b || !b->enabled) return 0;
    if (now_ms - b->last_toggle >= b->period_ms) {
        b->visible = !b->visible;
        b->last_toggle = now_ms;
        return 1;
    }
    return 0;
}
void blt_reset(BlinkTimer *b, long now_ms) { if (b) { b->visible = 1; b->last_toggle = now_ms; } }
int blt_visible(const BlinkTimer *b) { return b ? b->visible : 1; }
