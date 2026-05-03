/*
 * puttyalt_timeout.h: Per-session connection and idle timeout config.
 *
 * Allows setting a connect timeout (how long to wait for the initial
 * TCP handshake) and an idle timeout (auto-disconnect after N minutes
 * of inactivity).  Both are configurable per session.
 */

#ifndef PUTTYALT_TIMEOUT_H
#define PUTTYALT_TIMEOUT_H

typedef struct TimeoutConfig {
    int connect_timeout_s;   /* seconds, 0 = system default */
    int idle_timeout_s;      /* seconds, 0 = disabled */
    int warn_before_idle;    /* show warning N seconds before disconnect */
    int keepalive_interval;  /* override PuTTY's keepalive, 0 = use default */
} TimeoutConfig;

/* Defaults: 30s connect, no idle timeout */
void timeout_config_init(TimeoutConfig *tc);

/* Check if the connection has been idle too long.
 * Returns seconds remaining, or 0 if timed out, or -1 if disabled. */
int timeout_check_idle(const TimeoutConfig *tc, long last_activity, long now);

/* Check if we should show an idle warning.
 * Returns 1 if warning should be displayed. */
int timeout_should_warn(const TimeoutConfig *tc, long last_activity, long now);

/* Load/save timeout config from session INI section */
int timeout_load(TimeoutConfig *tc, const char *path, const char *session);
int timeout_save(const TimeoutConfig *tc, const char *path, const char *session);

#endif /* PUTTYALT_TIMEOUT_H */
