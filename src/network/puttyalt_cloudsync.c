/* puttyalt_cloudsync.c - Opt-in cloud session-sync transport state machine. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum cloudsync_state {
    CLOUDSYNC_DISABLED = 0,
    CLOUDSYNC_HANDSHAKE,
    CLOUDSYNC_AUTH,
    CLOUDSYNC_SYNCING,
    CLOUDSYNC_IDLE,
    CLOUDSYNC_BACKOFF,
    CLOUDSYNC_ERROR,
    CLOUDSYNC_STATE_COUNT
};

enum cloudsync_event {
    CLOUDSYNC_EV_CONNECTED = 0,
    CLOUDSYNC_EV_AUTH_OK,
    CLOUDSYNC_EV_SYNC_DONE,
    CLOUDSYNC_EV_WORK,
    CLOUDSYNC_EV_TRANSIENT_FAIL,
    CLOUDSYNC_EV_FATAL,
    CLOUDSYNC_EV_RETRY,
    CLOUDSYNC_EV_COUNT
};

enum cloudsync_err {
    CLOUDSYNC_OK = 0,
    CLOUDSYNC_ERR_NOT_ENABLED,
    CLOUDSYNC_ERR_BAD_TRANSITION,
    CLOUDSYNC_ERR_BAD_STATE,
    CLOUDSYNC_ERR_COUNT
};

#define CLOUDSYNC_BACKOFF_BASE_MS 500u
#define CLOUDSYNC_BACKOFF_MAX_MS  60000u
#define CLOUDSYNC_MAX_RETRY       8

struct cloudsync {
    int enabled;            /* opt-in gate: 0 until cloudsync_enable */
    int state;
    int last_err;
    unsigned retry;         /* consecutive transient failures */
    uint64_t backoff_until_ms;
};

static const char *const cloudsync_state_names[CLOUDSYNC_STATE_COUNT] = {
    "DISABLED", "HANDSHAKE", "AUTH", "SYNCING", "IDLE", "BACKOFF", "ERROR"
};

/* Transition table: 1 if from->to is permitted. */
static const unsigned char cloudsync_allowed[CLOUDSYNC_STATE_COUNT][CLOUDSYNC_STATE_COUNT] = {
    /* from DISABLED  */ {0,1,0,0,0,0,0},
    /* from HANDSHAKE */ {1,0,1,0,0,1,1},
    /* from AUTH      */ {1,0,0,1,0,1,1},
    /* from SYNCING   */ {1,0,0,0,1,1,1},
    /* from IDLE      */ {1,0,0,1,0,1,1},
    /* from BACKOFF   */ {1,1,0,0,0,0,1},
    /* from ERROR     */ {1,0,0,0,0,0,0}
};

void cloudsync_init(struct cloudsync *cs) {
    if (!cs) return;
    memset(cs, 0, sizeof(*cs));
    cs->enabled = 0;
    cs->state = CLOUDSYNC_DISABLED;
    cs->last_err = CLOUDSYNC_OK;
}

const char *cloudsync_state_name(int state) {
    if (state < 0 || state >= CLOUDSYNC_STATE_COUNT)
        return "INVALID";
    return cloudsync_state_names[state];
}

const char *cloudsync_strerror(int err) {
    switch (err) {
        case CLOUDSYNC_OK:              return "ok";
        case CLOUDSYNC_ERR_NOT_ENABLED: return "cloud sync not enabled";
        case CLOUDSYNC_ERR_BAD_TRANSITION: return "illegal state transition";
        case CLOUDSYNC_ERR_BAD_STATE:   return "invalid state";
        default:                        return "unknown error";
    }
}

int cloudsync_can_transition(const struct cloudsync *cs, int to) {
    if (!cs) return 0;
    if (cs->state < 0 || cs->state >= CLOUDSYNC_STATE_COUNT) return 0;
    if (to < 0 || to >= CLOUDSYNC_STATE_COUNT) return 0;
    return cloudsync_allowed[cs->state][to] ? 1 : 0;
}

int cloudsync_enable(struct cloudsync *cs) {
    if (!cs) return -1;
    cs->enabled = 1;
    cs->retry = 0;
    cs->last_err = CLOUDSYNC_OK;
    cs->backoff_until_ms = 0;
    cs->state = CLOUDSYNC_HANDSHAKE;
    return 0;
}

int cloudsync_disable(struct cloudsync *cs) {
    if (!cs) return -1;
    cs->enabled = 0;
    cs->retry = 0;
    cs->backoff_until_ms = 0;
    cs->state = CLOUDSYNC_DISABLED;
    cs->last_err = CLOUDSYNC_OK;
    return 0;
}

int cloudsync_transition(struct cloudsync *cs, int to) {
    if (!cs) return -1;
    if (to < 0 || to >= CLOUDSYNC_STATE_COUNT) {
        cs->last_err = CLOUDSYNC_ERR_BAD_STATE;
        return -1;
    }
    /* Opt-in gate: only DISABLED is reachable when not enabled. */
    if (!cs->enabled && to != CLOUDSYNC_DISABLED) {
        cs->last_err = CLOUDSYNC_ERR_NOT_ENABLED;
        return -1;
    }
    if (!cloudsync_can_transition(cs, to)) {
        cs->last_err = CLOUDSYNC_ERR_BAD_TRANSITION;
        return -1;
    }
    cs->state = to;
    cs->last_err = CLOUDSYNC_OK;
    return 0;
}

uint64_t cloudsync_next_backoff_ms(struct cloudsync *cs, uint64_t now_ms) {
    unsigned shift, n;
    uint64_t delay = CLOUDSYNC_BACKOFF_BASE_MS;
    if (!cs) return CLOUDSYNC_BACKOFF_BASE_MS;
    if (cs->retry < CLOUDSYNC_MAX_RETRY) cs->retry++;
    n = cs->retry ? cs->retry - 1u : 0u;
    for (shift = 0; shift < n && delay < CLOUDSYNC_BACKOFF_MAX_MS; shift++)
        delay <<= 1;
    if (delay > CLOUDSYNC_BACKOFF_MAX_MS) delay = CLOUDSYNC_BACKOFF_MAX_MS;
    cs->backoff_until_ms = now_ms + delay;
    return delay;
}

int cloudsync_on_event(struct cloudsync *cs, int event, uint64_t now_ms) {
    int target;
    if (!cs) return -1;
    if (!cs->enabled) { cs->last_err = CLOUDSYNC_ERR_NOT_ENABLED; return -1; }
    switch (event) {
        case CLOUDSYNC_EV_CONNECTED:      target = CLOUDSYNC_AUTH;    break;
        case CLOUDSYNC_EV_AUTH_OK:        target = CLOUDSYNC_SYNCING; break;
        case CLOUDSYNC_EV_SYNC_DONE:      target = CLOUDSYNC_IDLE;    break;
        case CLOUDSYNC_EV_WORK:           target = CLOUDSYNC_SYNCING; break;
        case CLOUDSYNC_EV_RETRY:          target = CLOUDSYNC_HANDSHAKE; break;
        case CLOUDSYNC_EV_TRANSIENT_FAIL:
            (void)cloudsync_next_backoff_ms(cs, now_ms);
            target = CLOUDSYNC_BACKOFF;
            break;
        case CLOUDSYNC_EV_FATAL:          target = CLOUDSYNC_ERROR;   break;
        default:
            cs->last_err = CLOUDSYNC_ERR_BAD_STATE;
            return -1;
    }
    if (event == CLOUDSYNC_EV_AUTH_OK || event == CLOUDSYNC_EV_SYNC_DONE)
        cs->retry = 0;
    return cloudsync_transition(cs, target);
}

int cloudsync_is_active(const struct cloudsync *cs) {
    if (!cs || !cs->enabled) return 0;
    return (cs->state == CLOUDSYNC_HANDSHAKE || cs->state == CLOUDSYNC_AUTH ||
            cs->state == CLOUDSYNC_SYNCING || cs->state == CLOUDSYNC_IDLE) ? 1 : 0;
}

int cloudsync_status_line(const struct cloudsync *cs, char *buf, size_t len) {
    int n;
    if (!buf || len == 0) return -1;
    if (!cs) { buf[0] = '\0'; return -1; }
    n = snprintf(buf, len, "cloudsync: %s%s retry=%u err=%s",
                 cloudsync_state_name(cs->state),
                 cs->enabled ? "" : " (off)",
                 cs->retry, cloudsync_strerror(cs->last_err));
    if (n < 0 || (size_t)n >= len) {
        buf[len - 1] = '\0';
        return -1;
    }
    return n;
}
