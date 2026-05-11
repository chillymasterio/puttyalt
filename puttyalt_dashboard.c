#include "puttyalt_dashboard.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void dashboard_init(Dashboard *d, int interval)
{
    memset(d, 0, sizeof(*d));
    d->refresh_interval_sec = interval > 0 ? interval : 5;
}

int dashboard_add_session(Dashboard *d, int id, const char *host, int port)
{
    if (d->count >= DASH_MAX_SESSIONS) return -1;
    DashSession *s = &d->sessions[d->count++];
    memset(s, 0, sizeof(*s));
    s->session_id = id;
    if (host) snprintf(s->hostname, sizeof(s->hostname), "%s", host);
    s->port = port;
    s->connected = 1;
    snprintf(s->status, sizeof(s->status), "Connected");
    return 0;
}

int dashboard_update(Dashboard *d, int id, double latency,
                     unsigned long in, unsigned long out, const char *status)
{
    for (int i = 0; i < d->count; i++) {
        if (d->sessions[i].session_id == id) {
            DashSession *s = &d->sessions[i];
            s->latency_ms = latency;
            s->bytes_in = in;
            s->bytes_out = out;
            if (status) snprintf(s->status, sizeof(s->status), "%s", status);
            return 0;
        }
    }
    return -1;
}

int dashboard_remove_session(Dashboard *d, int id)
{
    for (int i = 0; i < d->count; i++) {
        if (d->sessions[i].session_id == id) {
            for (int j = i; j < d->count - 1; j++)
                d->sessions[j] = d->sessions[j + 1];
            d->count--;
            return 0;
        }
    }
    return -1;
}

void dashboard_refresh(Dashboard *d)
{
    d->last_refresh = (long)time(NULL);
    d->total_bytes = 0;
    d->total_errors = 0;
    for (int i = 0; i < d->count; i++) {
        d->total_bytes += d->sessions[i].bytes_in + d->sessions[i].bytes_out;
        d->total_errors += d->sessions[i].error_count;
    }
}

const DashSession *dashboard_get(const Dashboard *d, int id)
{
    for (int i = 0; i < d->count; i++)
        if (d->sessions[i].session_id == id)
            return &d->sessions[i];
    return NULL;
}
