/* puttyalt_statacc.c - Streaming min/max/mean/variance accumulator.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <math.h>
typedef struct { long count; double sum, sumsq, min, max; } StatAcc;
void sta_init(StatAcc *s) { if (s) { s->count = 0; s->sum = s->sumsq = 0; s->min = 0; s->max = 0; } }
void sta_add(StatAcc *s, double x) {
    if (!s) return;
    if (s->count == 0) { s->min = s->max = x; }
    else { if (x < s->min) s->min = x; if (x > s->max) s->max = x; }
    s->count++; s->sum += x; s->sumsq += x * x;
}
double sta_mean(const StatAcc *s) { return (s && s->count) ? s->sum / s->count : 0.0; }
double sta_variance(const StatAcc *s) {
    if (!s || s->count < 2) return 0.0;
    double m = s->sum / s->count;
    return (s->sumsq - s->count * m * m) / (s->count - 1);
}
double sta_stddev(const StatAcc *s) { return sqrt(sta_variance(s)); }
