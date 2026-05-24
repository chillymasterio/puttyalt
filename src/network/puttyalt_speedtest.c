#include "puttyalt_speedtest.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void speedtest_init(SpeedTestResult *st)
{
    memset(st, 0, sizeof(*st));
    st->run_on_connect = 1;
    st->test_size_kb = 64;
}

void speedtest_calc_quality(SpeedTestResult *st)
{
    if (st->latency_ms < 50 && st->jitter_ms < 10 && st->packet_loss_pct == 0)
        snprintf(st->quality, sizeof(st->quality), "Excellent");
    else if (st->latency_ms < 150 && st->jitter_ms < 30 && st->packet_loss_pct <= 1)
        snprintf(st->quality, sizeof(st->quality), "Good");
    else if (st->latency_ms < 300 && st->packet_loss_pct <= 5)
        snprintf(st->quality, sizeof(st->quality), "Fair");
    else
        snprintf(st->quality, sizeof(st->quality), "Poor");
}

int speedtest_run_latency(SpeedTestResult *st, int sample_count)
{
    (void)sample_count;
    /* platform-specific: would measure RTT via echo */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(st->last_test, sizeof(st->last_test), "%Y-%m-%d %H:%M", t);
    st->test_count++;
    speedtest_calc_quality(st);
    return 0;
}

const char *speedtest_quality_label(SpeedTestResult *st) { return st->quality; }
