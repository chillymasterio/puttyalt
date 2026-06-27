/* puttyalt_scrollmomentum.c - Compute kinetic scroll momentum.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double velocity; double friction; double position; } ScrollMomentum;
void sm6_init(ScrollMomentum *s, double friction) {
    if (!s) return;
    s->velocity = 0; s->position = 0;
    s->friction = (friction > 0 && friction < 1) ? friction : 0.95;
}
void sm6_fling(ScrollMomentum *s, double velocity) { if (s) s->velocity = velocity; }
/* Advance one tick; returns 1 if still moving. */
int sm6_tick(ScrollMomentum *s) {
    if (!s) return 0;
    s->position += s->velocity;
    s->velocity *= s->friction;
    if (s->velocity < 0.5 && s->velocity > -0.5) { s->velocity = 0; return 0; }
    return 1;
}
double sm6_position(const ScrollMomentum *s) { return s ? s->position : 0; }
