#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

typedef struct {
    double latency_ms;
    double throughput_kbps;
    int packet_loss_pct;
    int samples;
} SpeedResult;

static double get_time_ms(void)
{
#ifdef _WIN32
    return (double)GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
#endif
}

void speedtest_init(SpeedResult *r)
{
    memset(r, 0, sizeof(*r));
}

void speedtest_add_sample(SpeedResult *r, double latency, double throughput)
{
    r->latency_ms = (r->latency_ms * r->samples + latency) / (r->samples + 1);
    r->throughput_kbps = (r->throughput_kbps * r->samples + throughput) / (r->samples + 1);
    r->samples++;
}

const char *speedtest_grade(SpeedResult *r)
{
    if (r->latency_ms < 20 && r->throughput_kbps > 10000) return "Excellent";
    if (r->latency_ms < 50 && r->throughput_kbps > 5000) return "Good";
    if (r->latency_ms < 150 && r->throughput_kbps > 1000) return "Fair";
    return "Poor";
}

int speedtest_format(SpeedResult *r, char *buf, int buflen)
{
    return snprintf(buf, buflen, "Latency: %.1fms | Speed: %.0f kbps | Grade: %s",
                    r->latency_ms, r->throughput_kbps, speedtest_grade(r));
}
