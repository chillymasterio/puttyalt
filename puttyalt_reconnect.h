/*
 * puttyalt_reconnect.h: Auto-reconnect logic for dropped connections.
 *
 * When a connection is lost unexpectedly (not a clean logout),
 * PuttyAlt can automatically attempt to reconnect with exponential
 * backoff. This is invaluable for sysadmins monitoring servers over
 * unreliable links.
 */

#ifndef PUTTYALT_RECONNECT_H
#define PUTTYALT_RECONNECT_H

/* Reconnect policy */
typedef enum {
    RECONNECT_NEVER,       /* Classic PuTTY behaviour */
    RECONNECT_ON_DROP,     /* Reconnect only on unexpected disconnect */
    RECONNECT_ALWAYS       /* Reconnect on any disconnect except user-initiated */
} ReconnectPolicy;

/* Reconnect state machine */
typedef enum {
    RSTATE_IDLE,
    RSTATE_WAITING,
    RSTATE_CONNECTING,
    RSTATE_CONNECTED,
    RSTATE_GAVE_UP
} ReconnectState;

typedef struct ReconnectCtx {
    ReconnectPolicy policy;
    ReconnectState state;

    int max_attempts;      /* 0 = unlimited */
    int attempt;
    int base_delay_ms;     /* Initial delay (default 1000) */
    int max_delay_ms;      /* Cap for exponential backoff (default 60000) */
    int current_delay_ms;

    /* Statistics */
    int total_reconnects;
    int total_failures;
} ReconnectCtx;

/* Initialise a reconnect context with default settings */
void reconnect_init(ReconnectCtx *ctx);

/* Notify the reconnect system that a connection was lost.
 * Returns true if a reconnect should be attempted. */
int reconnect_on_disconnect(ReconnectCtx *ctx, int was_clean_exit);

/* Called after a successful reconnect */
void reconnect_success(ReconnectCtx *ctx);

/* Called after a failed reconnect attempt */
void reconnect_failure(ReconnectCtx *ctx);

/* Get current delay before next reconnect attempt (ms) */
int reconnect_get_delay(const ReconnectCtx *ctx);

/* Reset the reconnect state (e.g., user opened a new session) */
void reconnect_reset(ReconnectCtx *ctx);

#endif /* PUTTYALT_RECONNECT_H */
