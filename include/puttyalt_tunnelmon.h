#ifndef PUTTYALT_TUNNELMON_H
#define PUTTYALT_TUNNELMON_H

#define TM_MAX_TUNNELS  32
#define TM_MAX_HOST     256

typedef enum {
    TM_STATE_IDLE = 0,
    TM_STATE_CONNECTING,
    TM_STATE_ACTIVE,
    TM_STATE_ERROR,
    TM_STATE_RECONNECTING
} TMState;

typedef struct {
    char    label[64];
    char    local_bind[TM_MAX_HOST];
    int     local_port;
    char    remote_host[TM_MAX_HOST];
    int     remote_port;
    char    jump_host[TM_MAX_HOST];
    TMState state;
    long    bytes_rx;
    long    bytes_tx;
    int     connections;
    long    uptime_sec;
    long    last_error;
    char    error_msg[128];
    int     auto_reconnect;
    int     max_retries;
    int     retry_count;
} TMTunnel;

typedef struct {
    TMTunnel tunnels[TM_MAX_TUNNELS];
    int      count;
    int      total_active;
    long     total_bytes;
} TunnelMonitor;

void tunmon_init(TunnelMonitor *tm);
int  tunmon_add(TunnelMonitor *tm, const char *label, const char *local,
                int lport, const char *remote, int rport, const char *jump);
int  tunmon_remove(TunnelMonitor *tm, int index);
int  tunmon_start(TunnelMonitor *tm, int index);
int  tunmon_stop(TunnelMonitor *tm, int index);
void tunmon_update(TunnelMonitor *tm);
int  tunmon_get_active(const TunnelMonitor *tm, int *indices, int max);
int  tunmon_export_status(const TunnelMonitor *tm, char *buf, int bufsz);

#endif
