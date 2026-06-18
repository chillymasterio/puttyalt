/* puttyalt_ema2.c - Exponential moving average tracker.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double value; double alpha; int initialized; } Ema2;
void em2_init(Ema2 *e, double alpha) {
    if (!e) return;
    e->alpha = (alpha > 0 && alpha <= 1) ? alpha : 0.1;
    e->value = 0; e->initialized = 0;
}
double em2_update(Ema2 *e, double sample) {
    if (!e) return 0;
    if (!e->initialized) { e->value = sample; e->initialized = 1; }
    else e->value = e->alpha * sample + (1 - e->alpha) * e->value;
    return e->value;
}
double em2_get(const Ema2 *e) { return e ? e->value : 0; }
