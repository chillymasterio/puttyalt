#ifndef PUTTYALT_DASHBOARD_H
#define PUTTYALT_DASHBOARD_H

#define DASH_MAX_SESSIONS 32

typedef struct {
    int session_id;
    char hostname[256];
    int port;
    int connected;
    double latency_ms;
    unsigned long bytes_in;
    unsigned long bytes_out;
    long uptime_sec;
    int error_count;
    char status[64];
} DashSession;

typedef struct {
    DashSession sessions[DASH_MAX_SESSIONS];
    int count;
    int refresh_interval_sec;
    long last_refresh;
    unsigned long total_bytes;
    int total_errors;
} Dashboard;

void dashboard_init(Dashboard *d, int refresh_interval);
int  dashboard_add_session(Dashboard *d, int id, const char *host, int port);
int  dashboard_update(Dashboard *d, int id, double latency,
                      unsigned long in, unsigned long out, const char *status);
int  dashboard_remove_session(Dashboard *d, int id);
void dashboard_refresh(Dashboard *d);
const DashSession *dashboard_get(const Dashboard *d, int id);

#endif
