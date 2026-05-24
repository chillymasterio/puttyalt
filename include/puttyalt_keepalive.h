#ifndef PUTTYALT_KEEPALIVE_H
#define PUTTYALT_KEEPALIVE_H

#define KA_MAX_SESSIONS  64

typedef enum {
    KA_SSH_NOOP = 0,
    KA_SSH_IGNORE,
    KA_NULL_BYTE,
    KA_CUSTOM
} KeepAliveMethod;

typedef struct KeepAliveConfig {
    int interval_sec;
    KeepAliveMethod method;
    char custom_data[64];
    int custom_len;
    int enabled;
    int count_sent;
    unsigned long last_sent;
} KeepAliveConfig;

typedef struct KeepAliveManager {
    KeepAliveConfig sessions[KA_MAX_SESSIONS];
    int count;
    int global_interval;
} KeepAliveManager;

void ka_init(KeepAliveManager *km);
int  ka_register(KeepAliveManager *km, int session_id, int interval_sec,
                 KeepAliveMethod method);
int  ka_unregister(KeepAliveManager *km, int session_id);
int  ka_tick(KeepAliveManager *km, unsigned long now);
void ka_set_global_interval(KeepAliveManager *km, int seconds);

#endif
