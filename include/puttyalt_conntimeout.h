#ifndef PUTTYALT_CONNTIMEOUT_H
#define PUTTYALT_CONNTIMEOUT_H

typedef struct {
    int connect_timeout_sec;
    int auth_timeout_sec;
    int idle_timeout_sec;
    int keepalive_interval_sec;
    int keepalive_max_retries;
    int tcp_nodelay;
    int network_timeout_sec;
    int auto_disconnect_idle;
    int warn_before_idle_disconnect;
    int warn_seconds_before;
} TimeoutConfig;

void timeout_config_default(TimeoutConfig *tc);
void timeout_config_fast(TimeoutConfig *tc);
void timeout_config_patient(TimeoutConfig *tc);
int  timeout_is_expired(TimeoutConfig *tc, int elapsed_sec, int type);

#endif
