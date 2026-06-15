/* puttyalt_movavg.c - Moving average / EMA over a window.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define MAV_WIN 64
typedef struct { double buf[MAV_WIN]; int n, head; double sum; } MovAvg;
void mav_init(MovAvg *m) { if (!m) return; m->n=0; m->head=0; m->sum=0; }
void mav_add(MovAvg *m, double x) {
    if (!m) return;
    if (m->n < MAV_WIN) { m->buf[m->head]=x; m->sum+=x; m->n++; }
    else { m->sum -= m->buf[m->head]; m->buf[m->head]=x; m->sum+=x; }
    m->head = (m->head+1) % MAV_WIN;
}
double mav_mean(const MovAvg *m) { return (m && m->n) ? m->sum/m->n : 0.0; }
double mav_ema(double prev, double x, double alpha) { return alpha*x + (1.0-alpha)*prev; }
