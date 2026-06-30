/* puttyalt_movingmedian.c - Approximate moving median.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define MM_WIN 15
typedef struct { double window[MM_WIN]; int n; int head; } MovingMedian;
void mm7_init(MovingMedian *m) { if (m) { m->n = 0; m->head = 0; } }
void mm7_add(MovingMedian *m, double v) {
    if (!m) return;
    m->window[m->head] = v;
    m->head = (m->head + 1) % MM_WIN;
    if (m->n < MM_WIN) m->n++;
}
double mm7_median(const MovingMedian *m) {
    if (!m || m->n == 0) return 0;
    double sorted[MM_WIN];
    for (int i = 0; i < m->n; i++) sorted[i] = m->window[i];
    for (int i = 0; i < m->n; i++) for (int j = i+1; j < m->n; j++)
        if (sorted[j] < sorted[i]) { double t = sorted[i]; sorted[i] = sorted[j]; sorted[j] = t; }
    return sorted[m->n / 2];
}
