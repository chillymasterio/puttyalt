/* puttyalt_gauge2.c - Gauge metric with min/max tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double value; double min; double max; int initialized; } Gauge2;
void gg2_init(Gauge2 *g) { if (g) { g->value = 0; g->min = 0; g->max = 0; g->initialized = 0; } }
void gg2_set(Gauge2 *g, double v) {
    if (!g) return;
    g->value = v;
    if (!g->initialized) { g->min = g->max = v; g->initialized = 1; }
    else { if (v < g->min) g->min = v; if (v > g->max) g->max = v; }
}
double gg2_value(const Gauge2 *g) { return g ? g->value : 0; }
double gg2_range(const Gauge2 *g) { return g && g->initialized ? g->max - g->min : 0; }
