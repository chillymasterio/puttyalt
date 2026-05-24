#ifndef PUTTYALT_SPEEDTEST_H
#define PUTTYALT_SPEEDTEST_H

typedef struct {
    int  enabled;
    int  run_on_connect;
    int  test_size_kb;
    double latency_ms;
    double upload_kbps;
    double download_kbps;
    double jitter_ms;
    int  packet_loss_pct;
    int  test_count;
    char last_test[32];
    char quality[16]; /* Excellent, Good, Fair, Poor */
} SpeedTestResult;

void speedtest_init(SpeedTestResult *st);
void speedtest_calc_quality(SpeedTestResult *st);
int  speedtest_run_latency(SpeedTestResult *st, int sample_count);
const char *speedtest_quality_label(SpeedTestResult *st);

#endif
