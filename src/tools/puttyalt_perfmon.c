#include "puttyalt_perfmon.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void perfmon_init(PerfMonitor *pm)
{
    memset(pm, 0, sizeof(*pm));
    pm->enabled = 1;
    pm->update_interval_ms = 1000;
    pm->start_time = (long)time(NULL);
}

int perfmon_add_metric(PerfMonitor *pm, const char *name, const char *unit,
                       double warn, double crit)
{
    if (pm->count >= PM_MAX_METRICS) return -1;
    PerfMetric *m = &pm->metrics[pm->count];
    memset(m, 0, sizeof(*m));
    snprintf(m->name, PM_MAX_NAME, "%s", name);
    snprintf(m->unit, sizeof(m->unit), "%s", unit);
    m->threshold_warn = warn;
    m->threshold_crit = crit;
    m->min_val = 1e18;
    m->max_val = -1e18;
    return pm->count++;
}

static int find_metric(const PerfMonitor *pm, const char *name)
{
    for (int i = 0; i < pm->count; i++)
        if (strcmp(pm->metrics[i].name, name) == 0) return i;
    return -1;
}

int perfmon_record(PerfMonitor *pm, const char *name, double value)
{
    if (!pm->enabled) return -1;
    int idx = find_metric(pm, name);
    if (idx < 0) return -1;
    PerfMetric *m = &pm->metrics[idx];
    m->samples[m->sample_pos] = value;
    m->sample_pos = (m->sample_pos + 1) % PM_MAX_SAMPLES;
    if (m->sample_count < PM_MAX_SAMPLES) m->sample_count++;
    m->current = value;
    if (value < m->min_val) m->min_val = value;
    if (value > m->max_val) m->max_val = value;
    return 0;
}

void perfmon_update(PerfMonitor *pm)
{
    pm->last_update = (long)time(NULL);
    for (int i = 0; i < pm->count; i++) {
        PerfMetric *m = &pm->metrics[i];
        if (m->sample_count == 0) continue;
        double sum = 0;
        int start = m->sample_count >= PM_MAX_SAMPLES ? m->sample_pos : 0;
        int total = m->sample_count < PM_MAX_SAMPLES ? m->sample_count : PM_MAX_SAMPLES;
        for (int j = 0; j < total; j++)
            sum += m->samples[(start + j) % PM_MAX_SAMPLES];
        m->avg_val = sum / total;
    }
}

int perfmon_check_alerts(const PerfMonitor *pm, int *indices, int max)
{
    int n = 0;
    for (int i = 0; i < pm->count && n < max; i++) {
        if (pm->metrics[i].current >= pm->metrics[i].threshold_crit ||
            pm->metrics[i].current >= pm->metrics[i].threshold_warn)
            indices[n++] = i;
    }
    return n;
}

int perfmon_format_summary(const PerfMonitor *pm, char *buf, int bufsz)
{
    int pos = 0;
    pos += snprintf(buf + pos, bufsz - pos, "Performance Monitor\n");
    for (int i = 0; i < pm->count && pos < bufsz - 100; i++) {
        const PerfMetric *m = &pm->metrics[i];
        const char *status = "OK";
        if (m->current >= m->threshold_crit) status = "CRIT";
        else if (m->current >= m->threshold_warn) status = "WARN";
        pos += snprintf(buf + pos, bufsz - pos,
                       "  %-20s %8.2f %s  (avg: %.2f, min: %.2f, max: %.2f) [%s]\n",
                       m->name, m->current, m->unit, m->avg_val, m->min_val, m->max_val, status);
    }
    return pos;
}

int perfmon_export(const PerfMonitor *pm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "metric,current,avg,min,max,unit\n");
    for (int i = 0; i < pm->count; i++) {
        const PerfMetric *m = &pm->metrics[i];
        fprintf(f, "%s,%.4f,%.4f,%.4f,%.4f,%s\n",
                m->name, m->current, m->avg_val, m->min_val, m->max_val, m->unit);
    }
    fclose(f);
    return 0;
}
