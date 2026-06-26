/* puttyalt_tcpwin.c - TCP congestion window estimation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int cwnd; int ssthresh; int in_slow_start; } TcpWin;
void tw2_init(TcpWin *w) { if (w) { w->cwnd = 1; w->ssthresh = 64; w->in_slow_start = 1; } }
/* On ACK: slow-start doubles, congestion-avoidance increments. */
void tw2_on_ack(TcpWin *w) {
    if (!w) return;
    if (w->in_slow_start) {
        w->cwnd++;
        if (w->cwnd >= w->ssthresh) w->in_slow_start = 0;
    } else {
        w->cwnd += 1; /* simplified additive increase per RTT */
    }
}
/* On loss: multiplicative decrease. */
void tw2_on_loss(TcpWin *w) {
    if (!w) return;
    w->ssthresh = w->cwnd / 2;
    if (w->ssthresh < 1) w->ssthresh = 1;
    w->cwnd = 1;
    w->in_slow_start = 1;
}
