#ifndef PUTTYALT_PERFMON_H
#define PUTTYALT_PERFMON_H

#define PM_MAX_METRICS  32
#define PM_MAX_SAMPLES  256
#define PM_MAX_NAME     64

typedef struct {
    char   name[PM_MAX_NAME];
    char   unit[16];
    double samples[PM_MAX_SAMPLES];
    int    sample_count;
    int    sample_pos;
    double min_val;
    double max_val;
    double avg_val;
    double current;
    double threshold_warn;
    double threshold_crit;
} PerfMetric;

typedef struct {
    PerfMetric metrics[PM_MAX_METRICS];
    int        count;
    int        enabled;
    long       start_time;
    long       last_update;
    int        update_interval_ms;
} PerfMonitor;

void perfmon_init(PerfMonitor *pm);
int  perfmon_add_metric(PerfMonitor *pm, const char *name, const char *unit,
                        double warn, double crit);
int  perfmon_record(PerfMonitor *pm, const char *name, double value);
void perfmon_update(PerfMonitor *pm);
int  perfmon_export(const PerfMonitor *pm, const char *path);
int  perfmon_format_summary(const PerfMonitor *pm, char *buf, int bufsz);
int  perfmon_check_alerts(const PerfMonitor *pm, int *indices, int max);

#endif
