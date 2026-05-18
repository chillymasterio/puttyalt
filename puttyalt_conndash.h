#ifndef PUTTYALT_CONNDASH_H
#define PUTTYALT_CONNDASH_H

#define DASH_MAX_SESSIONS 64

typedef enum {
    SESS_DISCONNECTED = 0,
    SESS_CONNECTING,
    SESS_CONNECTED,
    SESS_IDLE,
    SESS_ERROR,
    SESS_RECONNECTING
} SessionStatus;

typedef struct {
    int  id;
    char name[128];
    char host[256];
    int  port;
    SessionStatus status;
    int  latency_ms;
    unsigned long connected_since;
    unsigned long bytes_in;
    unsigned long bytes_out;
    int  tab_index;
    char error_msg[128];
} DashSession;

typedef struct {
    DashSession sessions[DASH_MAX_SESSIONS];
    int count;
    int visible;
    int sort_by; /* 0=name, 1=status, 2=latency, 3=traffic */
    int refresh_interval_sec;
} ConnDashboard;

void conndash_init(ConnDashboard *cd);
int  conndash_add(ConnDashboard *cd, int id, const char *name, const char *host, int port);
int  conndash_update_status(ConnDashboard *cd, int id, SessionStatus status);
int  conndash_update_stats(ConnDashboard *cd, int id, int latency, unsigned long in, unsigned long out);
DashSession *conndash_find(ConnDashboard *cd, int id);
void conndash_remove(ConnDashboard *cd, int id);
int  conndash_count_by_status(ConnDashboard *cd, SessionStatus status);
void conndash_sort(ConnDashboard *cd);
void conndash_toggle(ConnDashboard *cd);

#endif
