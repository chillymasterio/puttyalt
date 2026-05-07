#include <string.h>
#include "puttyalt_keepalive.h"

void ka_init(KeepAliveManager *km)
{
    memset(km, 0, sizeof(*km));
    km->global_interval = 60;
}

int ka_register(KeepAliveManager *km, int session_id, int interval_sec,
                KeepAliveMethod method)
{
    if (session_id < 0 || session_id >= KA_MAX_SESSIONS) return -1;
    KeepAliveConfig *c = &km->sessions[session_id];
    c->interval_sec = interval_sec > 0 ? interval_sec : km->global_interval;
    c->method = method;
    c->enabled = 1;
    c->count_sent = 0;
    if (session_id >= km->count) km->count = session_id + 1;
    return 0;
}

int ka_unregister(KeepAliveManager *km, int session_id)
{
    if (session_id < 0 || session_id >= km->count) return -1;
    km->sessions[session_id].enabled = 0;
    return 0;
}

int ka_tick(KeepAliveManager *km, unsigned long now)
{
    int sent = 0;
    for (int i = 0; i < km->count; i++) {
        KeepAliveConfig *c = &km->sessions[i];
        if (!c->enabled) continue;
        if (c->interval_sec <= 0) continue;
        if (now - c->last_sent >= (unsigned long)c->interval_sec) {
            c->last_sent = now;
            c->count_sent++;
            sent++;
            /* Actual send is handled by the session layer */
        }
    }
    return sent;
}

void ka_set_global_interval(KeepAliveManager *km, int seconds)
{
    km->global_interval = seconds > 0 ? seconds : 60;
}
