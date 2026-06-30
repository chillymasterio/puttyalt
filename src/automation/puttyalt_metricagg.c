/* puttyalt_metricagg.c - Aggregate metrics over a time window.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double sum; long count; double min; double max; long window_start; long window; } MetricAgg;
void ma2_init(MetricAgg *m, long window) {
    if (!m) return;
    m->sum = 0; m->count = 0; m->min = 0; m->max = 0; m->window_start = 0; m->window = window > 0 ? window : 60;
}
void ma2_record(MetricAgg *m, double v, long now) {
    if (!m) return;
    if (now - m->window_start >= m->window) { m->sum = 0; m->count = 0; m->min = m->max = v; m->window_start = now; }
    if (m->count == 0) { m->min = m->max = v; }
    else { if (v < m->min) m->min = v; if (v > m->max) m->max = v; }
    m->sum += v; m->count++;
}
double ma2_avg(const MetricAgg *m) { return (m && m->count) ? m->sum / m->count : 0; }
