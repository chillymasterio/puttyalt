#ifndef PUTTYALT_CONNPROF_H
#define PUTTYALT_CONNPROF_H

#define PROF_MAX_SAMPLES 256
#define PROF_MAX_PROFILES 32

typedef struct {
    unsigned long timestamp;
    double latency_ms;
    unsigned long bytes_sent;
    unsigned long bytes_recv;
    int packet_loss;
} ProfSample;

typedef struct {
    char hostname[256];
    int port;
    ProfSample samples[PROF_MAX_SAMPLES];
    int num_samples;
    int ring_pos;
    double avg_latency;
    double min_latency;
    double max_latency;
    double jitter;
    unsigned long total_bytes;
    unsigned long first_sample_time;
    unsigned long last_sample_time;
} ConnProfile;

typedef struct {
    ConnProfile profiles[PROF_MAX_PROFILES];
    int count;
    int sampling_interval_sec;
    int active;
} ConnectionProfiler;

void profiler_init(ConnectionProfiler *cp, int interval);
int  profiler_start(ConnectionProfiler *cp, const char *host, int port);
int  profiler_add_sample(ConnectionProfiler *cp, const char *host,
                         double latency, unsigned long sent, unsigned long recv);
void profiler_stop(ConnectionProfiler *cp, const char *host);
const ConnProfile *profiler_get(const ConnectionProfiler *cp, const char *host);
void profiler_recalculate(ConnProfile *p);
int  profiler_export_csv(const ConnProfile *p, const char *path);

#endif
