/*
 * puttyalt_broadcast.h: Multi-input broadcast — type once, send to
 * multiple sessions simultaneously.
 *
 * When enabled, keystrokes entered in one terminal are forwarded to
 * all other terminals in the broadcast group.  Useful for running the
 * same command on a fleet of servers at once.
 */

#ifndef PUTTYALT_BROADCAST_H
#define PUTTYALT_BROADCAST_H

#define MAX_BROADCAST_TARGETS  64

typedef struct BroadcastTarget {
    int   session_id;       /* opaque ID from the tab/session layer */
    char  label[128];       /* display label, e.g. "prod-web-01" */
    int   active;           /* 1 = receiving broadcasts */
} BroadcastTarget;

typedef struct BroadcastGroup {
    BroadcastTarget targets[MAX_BROADCAST_TARGETS];
    int  count;
    int  enabled;           /* master switch */
    int  echo_local;        /* also send to the originating terminal */
} BroadcastGroup;

/* Initialise a broadcast group */
void broadcast_init(BroadcastGroup *bg);

/* Add a session to the broadcast group.  Returns index or -1. */
int broadcast_add(BroadcastGroup *bg, int session_id, const char *label);

/* Remove a session by session_id */
int broadcast_remove(BroadcastGroup *bg, int session_id);

/* Toggle a single target on/off */
int broadcast_toggle(BroadcastGroup *bg, int session_id);

/* Enable / disable the whole group */
void broadcast_set_enabled(BroadcastGroup *bg, int enabled);

/* Get list of active target session IDs.
 * Writes up to max_out IDs into out[].  Returns count written. */
int broadcast_get_active(const BroadcastGroup *bg, int *out, int max_out);

/* Remove all targets */
void broadcast_clear(BroadcastGroup *bg);

#endif /* PUTTYALT_BROADCAST_H */
