#include "puttyalt_monitor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void monitor_init(SessionMonitor *mon, int interval)
{
    memset(mon, 0, sizeof(*mon));
    mon->check_interval_sec = interval > 0 ? interval : 10;
    mon->active = 1;
}

int monitor_add_rule(SessionMonitor *mon, const MonitorRule *rule)
{
    if (mon->num_rules >= MON_MAX_RULES) return -1;
    memcpy(&mon->rules[mon->num_rules++], rule, sizeof(MonitorRule));
    return 0;
}

int monitor_remove_rule(SessionMonitor *mon, const char *name)
{
    for (int i = 0; i < mon->num_rules; i++) {
        if (strcmp(mon->rules[i].name, name) == 0) {
            for (int j = i; j < mon->num_rules - 1; j++)
                mon->rules[j] = mon->rules[j + 1];
            mon->num_rules--;
            return 0;
        }
    }
    return -1;
}

static int eval_condition(MonOperator op, double value, double threshold)
{
    switch (op) {
    case MON_OP_GT: return value > threshold;
    case MON_OP_LT: return value < threshold;
    case MON_OP_EQ: return value == threshold;
    case MON_OP_NEQ: return value != threshold;
    }
    return 0;
}

int monitor_check(SessionMonitor *mon, MonMetric metric, double value)
{
    if (!mon->active) return 0;
    int triggered = 0;
    long now = (long)time(NULL);

    for (int i = 0; i < mon->num_rules; i++) {
        MonitorRule *r = &mon->rules[i];
        if (!r->enabled || r->metric != metric) continue;
        if (r->cooldown_sec > 0 && (now - r->last_triggered) < r->cooldown_sec) continue;

        if (eval_condition(r->op, value, r->threshold)) {
            r->last_triggered = now;

            MonitorAlert *a = &mon->alerts[mon->alert_ring % MON_MAX_ALERTS];
            a->timestamp = now;
            a->rule_index = i;
            a->value = value;
            snprintf(a->message, sizeof(a->message),
                     "[%s] %.2f triggered (threshold: %.2f)",
                     r->name, value, r->threshold);
            mon->alert_ring++;
            if (mon->num_alerts < MON_MAX_ALERTS) mon->num_alerts++;

            /* Exec actions are dispatched via the GUI event loop
             * to avoid direct process spawning from the library */

            triggered++;
        }
    }
    return triggered;
}

int monitor_get_alerts(const SessionMonitor *mon, MonitorAlert *out, int max)
{
    int n = mon->num_alerts < max ? mon->num_alerts : max;
    int start = (mon->alert_ring > MON_MAX_ALERTS) ? mon->alert_ring - MON_MAX_ALERTS : 0;
    for (int i = 0; i < n; i++)
        out[i] = mon->alerts[(start + i) % MON_MAX_ALERTS];
    return n;
}

void monitor_clear_alerts(SessionMonitor *mon)
{
    mon->num_alerts = 0;
    mon->alert_ring = 0;
}
