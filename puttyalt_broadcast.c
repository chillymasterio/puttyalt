/*
 * puttyalt_broadcast.c: Multi-input broadcast implementation.
 *
 * Manages a group of sessions that receive duplicated keystrokes.
 * The GUI layer calls broadcast_get_active() after each keystroke
 * and forwards the input to each listed session.
 */

#include "puttyalt_broadcast.h"
#include <string.h>
#include <stdio.h>

void broadcast_init(BroadcastGroup *bg)
{
    memset(bg, 0, sizeof(*bg));
    bg->echo_local = 1;   /* default: also send to originator */
}

int broadcast_add(BroadcastGroup *bg, int session_id, const char *label)
{
    if (bg->count >= MAX_BROADCAST_TARGETS)
        return -1;

    /* Reject duplicates */
    for (int i = 0; i < bg->count; i++) {
        if (bg->targets[i].session_id == session_id)
            return -1;
    }

    BroadcastTarget *t = &bg->targets[bg->count];
    t->session_id = session_id;
    t->active = 1;
    if (label) {
        snprintf(t->label, sizeof(t->label), "%s", label);
    } else {
        snprintf(t->label, sizeof(t->label), "Session %d", session_id);
    }

    return bg->count++;
}

int broadcast_remove(BroadcastGroup *bg, int session_id)
{
    for (int i = 0; i < bg->count; i++) {
        if (bg->targets[i].session_id == session_id) {
            for (int j = i; j < bg->count - 1; j++)
                bg->targets[j] = bg->targets[j + 1];
            bg->count--;
            return 0;
        }
    }
    return -1;
}

int broadcast_toggle(BroadcastGroup *bg, int session_id)
{
    for (int i = 0; i < bg->count; i++) {
        if (bg->targets[i].session_id == session_id) {
            bg->targets[i].active = !bg->targets[i].active;
            return bg->targets[i].active;
        }
    }
    return -1;
}

void broadcast_set_enabled(BroadcastGroup *bg, int enabled)
{
    bg->enabled = enabled ? 1 : 0;
}

int broadcast_get_active(const BroadcastGroup *bg, int *out, int max_out)
{
    if (!bg->enabled)
        return 0;

    int n = 0;
    for (int i = 0; i < bg->count && n < max_out; i++) {
        if (bg->targets[i].active)
            out[n++] = bg->targets[i].session_id;
    }
    return n;
}

void broadcast_clear(BroadcastGroup *bg)
{
    bg->count = 0;
    bg->enabled = 0;
}
