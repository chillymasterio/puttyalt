#include <string.h>
#include <stdio.h>
#include <time.h>
#include "puttyalt_perftimer.h"

static unsigned long now_us(void)
{
#ifndef _WIN32
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000UL + ts.tv_nsec / 1000;
#else
    return 0; /* Windows: use QueryPerformanceCounter */
#endif
}

void perf_init(PerfProfiler *pp)
{
    memset(pp, 0, sizeof(*pp));
}

int perf_create(PerfProfiler *pp, const char *label)
{
    if (pp->count >= PERF_MAX_TIMERS) return -1;
    PerfTimer *t = &pp->timers[pp->count];
    memset(t, 0, sizeof(*t));
    strncpy(t->label, label, PERF_LABEL_LEN - 1);
    t->min_us = (unsigned long)-1;
    return pp->count++;
}

void perf_start(PerfProfiler *pp, int id)
{
    if (id < 0 || id >= pp->count) return;
    pp->timers[id].start_us = now_us();
    pp->timers[id].running = 1;
}

void perf_stop(PerfProfiler *pp, int id)
{
    if (id < 0 || id >= pp->count) return;
    PerfTimer *t = &pp->timers[id];
    if (!t->running) return;

    t->elapsed_us = now_us() - t->start_us;
    t->running = 0;
    t->count++;
    t->total_us += t->elapsed_us;
    if (t->elapsed_us < t->min_us) t->min_us = t->elapsed_us;
    if (t->elapsed_us > t->max_us) t->max_us = t->elapsed_us;
}

unsigned long perf_get_avg(const PerfProfiler *pp, int id)
{
    if (id < 0 || id >= pp->count) return 0;
    const PerfTimer *t = &pp->timers[id];
    return t->count > 0 ? t->total_us / t->count : 0;
}

void perf_report(const PerfProfiler *pp, char *buf, int buflen)
{
    int n = 0;
    n += snprintf(buf + n, buflen - n, "%-20s %8s %8s %8s %8s %6s\n",
                  "Timer", "Avg(us)", "Min(us)", "Max(us)", "Total(us)", "Count");
    for (int i = 0; i < pp->count && n < buflen; i++) {
        const PerfTimer *t = &pp->timers[i];
        unsigned long avg = t->count > 0 ? t->total_us / t->count : 0;
        n += snprintf(buf + n, buflen - n, "%-20s %8lu %8lu %8lu %8lu %6d\n",
                      t->label, avg, t->min_us == (unsigned long)-1 ? 0 : t->min_us,
                      t->max_us, t->total_us, t->count);
    }
}

void perf_reset(PerfProfiler *pp, int id)
{
    if (id < 0 || id >= pp->count) return;
    PerfTimer *t = &pp->timers[id];
    char label[PERF_LABEL_LEN];
    memcpy(label, t->label, PERF_LABEL_LEN);
    memset(t, 0, sizeof(*t));
    memcpy(t->label, label, PERF_LABEL_LEN);
    t->min_us = (unsigned long)-1;
}
