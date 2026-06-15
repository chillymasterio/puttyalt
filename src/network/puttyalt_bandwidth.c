/* puttyalt_bandwidth.c - Track throughput and format bit rates.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
typedef struct { long bytes; long window_start; long last_rate; } BwMeter;
void bw_init(BwMeter *m, long now) { if (m) { m->bytes = 0; m->window_start = now; m->last_rate = 0; } }
void bw_record(BwMeter *m, long bytes) { if (m) m->bytes += bytes; }
/* Compute bytes/sec over the elapsed window and reset it. */
long bw_rate(BwMeter *m, long now) {
    if (!m) return 0;
    long dt = now - m->window_start;
    if (dt <= 0) return m->last_rate;
    m->last_rate = m->bytes / dt;
    m->bytes = 0; m->window_start = now;
    return m->last_rate;
}
int bw_format(long bytes_per_sec, char *out, int outlen) {
    double bits = bytes_per_sec * 8.0;
    if (bits < 1000) return snprintf(out, outlen, "%.0f bps", bits);
    if (bits < 1e6) return snprintf(out, outlen, "%.1f Kbps", bits/1e3);
    if (bits < 1e9) return snprintf(out, outlen, "%.1f Mbps", bits/1e6);
    return snprintf(out, outlen, "%.1f Gbps", bits/1e9);
}
