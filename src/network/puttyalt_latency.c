/*
 * puttyalt_latency.c: Latency tracking implementation.
 */

#include "puttyalt_latency.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

void latency_init(LatencyTracker *lt)
{
    memset(lt, 0, sizeof(*lt));
    lt->min_ms = 999999.0;
}

void latency_add_sample(LatencyTracker *lt, double ms)
{
    int idx = (lt->head + lt->count) % LATENCY_HISTORY_SIZE;
    if (lt->count >= LATENCY_HISTORY_SIZE) {
        lt->head = (lt->head + 1) % LATENCY_HISTORY_SIZE;
    } else {
        lt->count++;
    }
    lt->samples[idx] = ms;
    latency_recalc(lt);
}

void latency_add_timeout(LatencyTracker *lt)
{
    latency_add_sample(lt, -1.0);  /* -1 marks a timeout */
    lt->loss_count++;
}

void latency_recalc(LatencyTracker *lt)
{
    double sum = 0;
    double sum_sq = 0;
    int valid = 0;

    lt->min_ms = 999999.0;
    lt->max_ms = 0;
    lt->loss_count = 0;

    for (int i = 0; i < lt->count; i++) {
        int idx = (lt->head + i) % LATENCY_HISTORY_SIZE;
        double s = lt->samples[idx];
        if (s < 0) {
            lt->loss_count++;
            continue;
        }
        sum += s;
        sum_sq += s * s;
        if (s < lt->min_ms) lt->min_ms = s;
        if (s > lt->max_ms) lt->max_ms = s;
        valid++;
    }

    if (valid > 0) {
        lt->avg_ms = sum / valid;
        double variance = (sum_sq / valid) - (lt->avg_ms * lt->avg_ms);
        lt->jitter_ms = (variance > 0) ? sqrt(variance) : 0;
    }
}

void latency_sparkline(const LatencyTracker *lt, char *buf, int bufsize)
{
    /* Unicode sparkline chars: ▁▂▃▄▅▆▇█ */
    static const char *blocks[] = {
        "\xe2\x96\x81", "\xe2\x96\x82", "\xe2\x96\x83", "\xe2\x96\x84",
        "\xe2\x96\x85", "\xe2\x96\x86", "\xe2\x96\x87", "\xe2\x96\x88"
    };

    if (lt->count == 0 || bufsize < 2) {
        buf[0] = '\0';
        return;
    }

    /* Show last 8 samples */
    int n = lt->count < 8 ? lt->count : 8;
    int start = lt->count - n;
    buf[0] = '\0';

    double range = lt->max_ms - lt->min_ms;
    if (range < 1.0) range = 1.0;

    for (int i = 0; i < n; i++) {
        int idx = (lt->head + start + i) % LATENCY_HISTORY_SIZE;
        double s = lt->samples[idx];
        if (s < 0) {
            strncat(buf, "X", bufsize - strlen(buf) - 1);
        } else {
            int level = (int)(7.0 * (s - lt->min_ms) / range);
            if (level < 0) level = 0;
            if (level > 7) level = 7;
            strncat(buf, blocks[level], bufsize - strlen(buf) - 1);
        }
    }
}

int latency_quality(const LatencyTracker *lt)
{
    if (lt->count == 0) return 0;
    double loss_pct = (lt->count > 0) ?
        (100.0 * lt->loss_count / lt->count) : 0;

    if (loss_pct > 10.0) return 4;
    /* critical */
    if (lt->avg_ms > 500.0) return 4;
    if (loss_pct > 5.0) return 3;
    /* poor */
    if (lt->avg_ms > 200.0) return 3;
    if (lt->jitter_ms > 100.0) return 2;
    /* fair */
    if (lt->avg_ms > 100.0) return 2;
    if (lt->avg_ms > 50.0) return 1;
    /* good */
    return 0;                             /* excellent */
}
