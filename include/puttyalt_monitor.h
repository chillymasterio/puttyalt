#ifndef PUTTYALT_MONITOR_H
#define PUTTYALT_MONITOR_H

#define MON_MAX_ALERTS 64
#define MON_MAX_RULES  32

typedef enum {
    MON_METRIC_LATENCY,
    MON_METRIC_BANDWIDTH,
    MON_METRIC_ERRORS,
    MON_METRIC_IDLE_TIME,
    MON_METRIC_MEMORY,
    MON_METRIC_UPTIME
} MonMetric;

typedef enum {
    MON_OP_GT,
    MON_OP_LT,
    MON_OP_EQ,
    MON_OP_NEQ
} MonOperator;

typedef enum {
    MON_ACT_NOTIFY,
    MON_ACT_LOG,
    MON_ACT_SOUND,
    MON_ACT_RECONNECT,
    MON_ACT_EXEC
} MonAction;

typedef struct {
    char name[64];
    MonMetric metric;
    MonOperator op;
    double threshold;
    MonAction action;
    char exec_cmd[256];
    int enabled;
    int cooldown_sec;
    long last_triggered;
} MonitorRule;

typedef struct {
    long timestamp;
    int rule_index;
    double value;
    char message[256];
} MonitorAlert;

typedef struct {
    MonitorRule rules[MON_MAX_RULES];
    int num_rules;
    MonitorAlert alerts[MON_MAX_ALERTS];
    int num_alerts;
    int alert_ring;
    int active;
    int check_interval_sec;
} SessionMonitor;

void monitor_init(SessionMonitor *mon, int interval);
int  monitor_add_rule(SessionMonitor *mon, const MonitorRule *rule);
int  monitor_remove_rule(SessionMonitor *mon, const char *name);
int  monitor_check(SessionMonitor *mon, MonMetric metric, double value);
int  monitor_get_alerts(const SessionMonitor *mon, MonitorAlert *out, int max);
void monitor_clear_alerts(SessionMonitor *mon);

#endif
