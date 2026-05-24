/*
 * puttyalt_latency.h: Connection latency tracker and sparkline graph.
 *
 * Records per-second RTT samples and renders a mini sparkline
 * graph in the status bar showing connection quality over time.
 */

#ifndef PUTTYALT_LATENCY_H
#define PUTTYALT_LATENCY_H

#define LATENCY_HISTORY_SIZE 120  /* 2 minutes of 1-second samples */

typedef struct LatencyTracker {
    double samples[LATENCY_HISTORY_SIZE];
    int    head;
    int    count;
    double min_ms;
    double max_ms;
    double avg_ms;
    double jitter_ms;       /* stddev of recent samples */
    int    loss_count;      /* timeouts in the window */
} LatencyTracker;

void latency_init(LatencyTracker *lt);
void latency_add_sample(LatencyTracker *lt, double ms);
void latency_add_timeout(LatencyTracker *lt);
void latency_recalc(LatencyTracker *lt);

/* Render a sparkline string (8 chars) representing recent latency */
void latency_sparkline(const LatencyTracker *lt, char *buf, int bufsize);

/* Get quality rating: 0=excellent, 1=good, 2=fair, 3=poor, 4=critical */
int latency_quality(const LatencyTracker *lt);

#endif /* PUTTYALT_LATENCY_H */
