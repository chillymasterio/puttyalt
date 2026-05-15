#include "puttyalt_autoreconn.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void reconn_init(AutoReconnect *ar)
{
    memset(ar, 0, sizeof(*ar));
    ar->enabled = 1;
    ar->max_attempts = 10;
    ar->base_delay_ms = 1000;
    ar->max_delay_ms = 30000;
    ar->current_delay_ms = 1000;
    ar->jitter_pct = 20;
}

void reconn_configure(AutoReconnect *ar, int max_attempts, int base_delay, int max_delay)
{
    ar->max_attempts = max_attempts > 0 ? max_attempts : 10;
    ar->base_delay_ms = base_delay > 0 ? base_delay : 1000;
    ar->max_delay_ms = max_delay > 0 ? max_delay : 30000;
}

void reconn_start(AutoReconnect *ar, const char *host, int port, const char *user)
{
    if (!ar->enabled) { ar->state = RECONN_DISABLED; return; }
    snprintf(ar->last_host, sizeof(ar->last_host), "%s", host);
    ar->last_port = port;
    if (user) snprintf(ar->last_user, sizeof(ar->last_user), "%s", user);
    ar->attempts = 0;
    ar->current_delay_ms = ar->base_delay_ms;
    ar->last_disconnect_time = (unsigned long)time(NULL);
    ar->state = RECONN_WAITING;
    ar->next_attempt_time = ar->last_disconnect_time * 1000 + ar->current_delay_ms;
}

void reconn_stop(AutoReconnect *ar) { ar->state = RECONN_IDLE; ar->attempts = 0; }

void reconn_success(AutoReconnect *ar)
{
    ar->state = RECONN_IDLE;
    ar->attempts = 0;
    ar->current_delay_ms = ar->base_delay_ms;
}

void reconn_failure(AutoReconnect *ar)
{
    ar->attempts++;
    if (ar->attempts >= ar->max_attempts) { ar->state = RECONN_DISABLED; return; }
    /* Exponential backoff with jitter */
    ar->current_delay_ms *= 2;
    if (ar->current_delay_ms > ar->max_delay_ms) ar->current_delay_ms = ar->max_delay_ms;
    int jitter = ar->current_delay_ms * ar->jitter_pct / 100;
    if (jitter > 0) ar->current_delay_ms += (rand() % (jitter * 2)) - jitter;
    ar->next_attempt_time = (unsigned long)time(NULL) * 1000 + ar->current_delay_ms;
    ar->state = RECONN_BACKOFF;
    if (ar->on_reconnect) ar->on_reconnect(ar->attempts, ar->callback_ctx);
}

int reconn_should_try(AutoReconnect *ar)
{
    if (ar->state != RECONN_WAITING && ar->state != RECONN_BACKOFF) return 0;
    unsigned long now = (unsigned long)time(NULL) * 1000;
    return now >= ar->next_attempt_time;
}

int reconn_time_remaining_ms(AutoReconnect *ar)
{
    unsigned long now = (unsigned long)time(NULL) * 1000;
    if (now >= ar->next_attempt_time) return 0;
    return (int)(ar->next_attempt_time - now);
}

const char *reconn_state_str(ReconnState s)
{
    switch (s) {
    case RECONN_IDLE: return "Idle";
    case RECONN_WAITING: return "Waiting";
    case RECONN_CONNECTING: return "Connecting";
    case RECONN_BACKOFF: return "Backoff";
    case RECONN_DISABLED: return "Disabled";
    }
    return "Unknown";
}
