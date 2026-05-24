#include "puttyalt_connprof.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void profiler_init(ConnectionProfiler *cp, int interval)
{
    memset(cp, 0, sizeof(*cp));
    cp->sampling_interval_sec = interval > 0 ? interval : 5;
}

static ConnProfile *find_or_create(ConnectionProfiler *cp, const char *host, int port)
{
    for (int i = 0; i < cp->count; i++)
        if (strcmp(cp->profiles[i].hostname, host) == 0 &&
            cp->profiles[i].port == port)
            return &cp->profiles[i];
    if (cp->count >= PROF_MAX_PROFILES) return NULL;
    ConnProfile *p = &cp->profiles[cp->count++];
    memset(p, 0, sizeof(*p));
    snprintf(p->hostname, sizeof(p->hostname), "%s", host);
    p->port = port;
    p->min_latency = 1e9;
    return p;
}

int profiler_start(ConnectionProfiler *cp, const char *host, int port)
{
    ConnProfile *p = find_or_create(cp, host, port);
    if (!p) return -1;
    p->first_sample_time = (unsigned long)time(NULL);
    cp->active = 1;
    return 0;
}

int profiler_add_sample(ConnectionProfiler *cp, const char *host,
                        double latency, unsigned long sent, unsigned long recv)
{
    ConnProfile *p = NULL;
    for (int i = 0; i < cp->count; i++)
        if (strcmp(cp->profiles[i].hostname, host) == 0)
            { p = &cp->profiles[i]; break; }
    if (!p) return -1;

    ProfSample *s = &p->samples[p->ring_pos % PROF_MAX_SAMPLES];
    s->timestamp = (unsigned long)time(NULL);
    s->latency_ms = latency;
    s->bytes_sent = sent;
    s->bytes_recv = recv;
    p->ring_pos++;
    if (p->num_samples < PROF_MAX_SAMPLES) p->num_samples++;
    p->last_sample_time = s->timestamp;
    p->total_bytes += sent + recv;

    profiler_recalculate(p);
    return 0;
}

void profiler_stop(ConnectionProfiler *cp, const char *host)
{
    (void)host;
    cp->active = 0;
}

const ConnProfile *profiler_get(const ConnectionProfiler *cp, const char *host)
{
    for (int i = 0; i < cp->count; i++)
        if (strcmp(cp->profiles[i].hostname, host) == 0)
            return &cp->profiles[i];
    return NULL;
}

void profiler_recalculate(ConnProfile *p)
{
    if (p->num_samples == 0) return;
    double sum = 0, min_l = 1e9, max_l = 0;
    int start = (p->ring_pos > PROF_MAX_SAMPLES) ? p->ring_pos - PROF_MAX_SAMPLES : 0;
    for (int i = 0; i < p->num_samples; i++) {
        int idx = (start + i) % PROF_MAX_SAMPLES;
        double l = p->samples[idx].latency_ms;
        sum += l;
        if (l < min_l) min_l = l;
        if (l > max_l) max_l = l;
    }
    p->avg_latency = sum / p->num_samples;
    p->min_latency = min_l;
    p->max_latency = max_l;

    /* Jitter: mean deviation from average */
    double jsum = 0;
    for (int i = 0; i < p->num_samples; i++) {
        int idx = (start + i) % PROF_MAX_SAMPLES;
        jsum += fabs(p->samples[idx].latency_ms - p->avg_latency);
    }
    p->jitter = jsum / p->num_samples;
}

int profiler_export_csv(const ConnProfile *p, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "timestamp,latency_ms,bytes_sent,bytes_recv\n");
    int start = (p->ring_pos > PROF_MAX_SAMPLES) ? p->ring_pos - PROF_MAX_SAMPLES : 0;
    for (int i = 0; i < p->num_samples; i++) {
        int idx = (start + i) % PROF_MAX_SAMPLES;
        const ProfSample *s = &p->samples[idx];
        fprintf(f, "%lu,%.2f,%lu,%lu\n", s->timestamp, s->latency_ms,
                s->bytes_sent, s->bytes_recv);
    }
    fclose(f);
    return 0;
}
