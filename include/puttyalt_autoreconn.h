#ifndef PUTTYALT_AUTORECONN_H
#define PUTTYALT_AUTORECONN_H

typedef enum { RECONN_IDLE, RECONN_WAITING, RECONN_CONNECTING, RECONN_BACKOFF, RECONN_DISABLED } ReconnState;

typedef struct {
    ReconnState state;
    int enabled;
    int attempts;
    int max_attempts;
    int base_delay_ms;
    int max_delay_ms;
    int current_delay_ms;
    int jitter_pct;
    unsigned long next_attempt_time;
    unsigned long last_disconnect_time;
    char last_host[256];
    int last_port;
    char last_user[128];
    void (*on_reconnect)(int attempt, void *ctx);
    void *callback_ctx;
} AutoReconnect;

void reconn_init(AutoReconnect *ar);
void reconn_configure(AutoReconnect *ar, int max_attempts, int base_delay, int max_delay);
void reconn_start(AutoReconnect *ar, const char *host, int port, const char *user);
void reconn_stop(AutoReconnect *ar);
void reconn_success(AutoReconnect *ar);
void reconn_failure(AutoReconnect *ar);
int  reconn_should_try(AutoReconnect *ar);
int  reconn_time_remaining_ms(AutoReconnect *ar);
const char *reconn_state_str(ReconnState s);

#endif
