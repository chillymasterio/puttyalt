#include "puttyalt_healthchk.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

void health_init(HealthInfo *hi)
{
    memset(hi, 0, sizeof(*hi));
    hi->status = HEALTH_UNKNOWN;
    hi->last_activity = (unsigned long)time(NULL);
}

void health_update(HealthInfo *hi, int latency_ms, int packet_lost)
{
    hi->latency_ms = latency_ms;
    hi->packets_sent++;
    if (packet_lost) hi->packets_lost++;
    hi->loss_pct = hi->packets_sent > 0 ? (float)hi->packets_lost / hi->packets_sent * 100.0f : 0.0f;
    hi->uptime_sec = (unsigned long)time(NULL) - hi->last_activity + hi->uptime_sec;
}

void health_record_bytes(HealthInfo *hi, unsigned long in_b, unsigned long out_b)
{
    hi->bytes_in += in_b;
    hi->bytes_out += out_b;
    health_record_activity(hi);
}

void health_record_activity(HealthInfo *hi)
{
    hi->last_activity = (unsigned long)time(NULL);
    hi->idle_sec = 0;
}

HealthStatus health_evaluate(HealthInfo *hi, const HealthConfig *cfg)
{
    HealthStatus prev = hi->status;
    hi->idle_sec = (int)((unsigned long)time(NULL) - hi->last_activity);

    if (hi->latency_ms >= cfg->crit_latency_ms || hi->loss_pct > 20.0f)
        hi->status = HEALTH_CRIT;
    else if (hi->latency_ms >= cfg->warn_latency_ms || hi->loss_pct > 5.0f || hi->idle_sec > cfg->warn_idle_sec)
        hi->status = HEALTH_WARN;
    else
        hi->status = HEALTH_OK;

    if (hi->reconnect_count > cfg->max_reconnects) hi->status = HEALTH_CRIT;

    snprintf(hi->message, sizeof(hi->message), "Latency: %dms | Loss: %.1f%% | Idle: %ds",
             hi->latency_ms, hi->loss_pct, hi->idle_sec);

    if (prev != hi->status && cfg->on_status_change)
        cfg->on_status_change(prev, hi->status, cfg->callback_ctx);

    return hi->status;
}

const char *health_status_str(HealthStatus s)
{
    switch (s) {
    case HEALTH_OK: return "OK";
    case HEALTH_WARN: return "Warning";
    case HEALTH_CRIT: return "Critical";
    default: return "Unknown";
    }
}

float health_uptime_hours(HealthInfo *hi) { return (float)hi->uptime_sec / 3600.0f; }
