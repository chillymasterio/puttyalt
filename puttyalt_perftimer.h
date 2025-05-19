#ifndef PUTTYALT_PERFTIMER_H
#define PUTTYALT_PERFTIMER_H

#define PERF_MAX_TIMERS  16
#define PERF_LABEL_LEN   32

typedef struct PerfTimer {
    char label[PERF_LABEL_LEN];
    unsigned long start_us;
    unsigned long elapsed_us;
    int running;
    int count;
    unsigned long total_us;
    unsigned long min_us;
    unsigned long max_us;
} PerfTimer;

typedef struct PerfProfiler {
    PerfTimer timers[PERF_MAX_TIMERS];
    int count;
} PerfProfiler;

void perf_init(PerfProfiler *pp);
int  perf_create(PerfProfiler *pp, const char *label);
void perf_start(PerfProfiler *pp, int id);
void perf_stop(PerfProfiler *pp, int id);
unsigned long perf_get_avg(const PerfProfiler *pp, int id);
void perf_report(const PerfProfiler *pp, char *buf, int buflen);
void perf_reset(PerfProfiler *pp, int id);

#endif
