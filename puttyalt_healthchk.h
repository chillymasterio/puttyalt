#ifndef PUTTYALT_HEALTHCHK_H
#define PUTTYALT_HEALTHCHK_H

typedef enum { HEALTH_OK, HEALTH_WARN, HEALTH_CRIT, HEALTH_UNKNOWN } HealthStatus;

typedef struct {
    HealthStatus status;
    int latency_ms;
    int packets_sent;
    int packets_lost;
    float loss_pct;
    unsigned long bytes_in;
    unsigned long bytes_out;
    unsigned long uptime_sec;
    unsigned long last_activity;
    int idle_sec;
    int reconnect_count;
    char message[256];
} HealthInfo;

typedef struct {
    int enabled;
    int check_interval_sec;
    int warn_latency_ms;
    int crit_latency_ms;
    int warn_idle_sec;
    int max_reconnects;
    void (*on_status_change)(HealthStatus old_s, HealthStatus new_s, void *ctx);
    void *callback_ctx;
} HealthConfig;

void health_init(HealthInfo *hi);
void health_update(HealthInfo *hi, int latency_ms, int packet_lost);
void health_record_bytes(HealthInfo *hi, unsigned long in_bytes, unsigned long out_bytes);
void health_record_activity(HealthInfo *hi);
HealthStatus health_evaluate(HealthInfo *hi, const HealthConfig *cfg);
const char *health_status_str(HealthStatus s);
float health_uptime_hours(HealthInfo *hi);

#endif
