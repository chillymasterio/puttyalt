#include "puttyalt_tunnelmon.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void tunmon_init(TunnelMonitor *tm)
{
    memset(tm, 0, sizeof(*tm));
}

int tunmon_add(TunnelMonitor *tm, const char *label, const char *local,
               int lport, const char *remote, int rport, const char *jump)
{
    if (tm->count >= TM_MAX_TUNNELS) return -1;
    TMTunnel *t = &tm->tunnels[tm->count];
    memset(t, 0, sizeof(*t));
    snprintf(t->label, sizeof(t->label), "%s", label);
    if (local) snprintf(t->local_bind, TM_MAX_HOST, "%s", local);
    t->local_port = lport;
    snprintf(t->remote_host, TM_MAX_HOST, "%s", remote);
    t->remote_port = rport;
    if (jump) snprintf(t->jump_host, TM_MAX_HOST, "%s", jump);
    t->auto_reconnect = 1;
    t->max_retries = 5;
    return tm->count++;
}

int tunmon_remove(TunnelMonitor *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    if (tm->tunnels[index].state == TM_STATE_ACTIVE) tm->total_active--;
    for (int i = index; i < tm->count - 1; i++)
        tm->tunnels[i] = tm->tunnels[i + 1];
    tm->count--;
    return 0;
}

int tunmon_start(TunnelMonitor *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    tm->tunnels[index].state = TM_STATE_ACTIVE;
    tm->tunnels[index].retry_count = 0;
    tm->total_active++;
    return 0;
}

int tunmon_stop(TunnelMonitor *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    if (tm->tunnels[index].state == TM_STATE_ACTIVE) tm->total_active--;
    tm->tunnels[index].state = TM_STATE_IDLE;
    return 0;
}

void tunmon_update(TunnelMonitor *tm)
{
    tm->total_bytes = 0;
    tm->total_active = 0;
    for (int i = 0; i < tm->count; i++) {
        TMTunnel *t = &tm->tunnels[i];
        tm->total_bytes += t->bytes_rx + t->bytes_tx;
        if (t->state == TM_STATE_ACTIVE) {
            tm->total_active++;
            t->uptime_sec++;
        }
        if (t->state == TM_STATE_ERROR && t->auto_reconnect &&
            t->retry_count < t->max_retries) {
            t->state = TM_STATE_RECONNECTING;
            t->retry_count++;
        }
    }
}

int tunmon_get_active(const TunnelMonitor *tm, int *indices, int max)
{
    int n = 0;
    for (int i = 0; i < tm->count && n < max; i++)
        if (tm->tunnels[i].state == TM_STATE_ACTIVE) indices[n++] = i;
    return n;
}

int tunmon_export_status(const TunnelMonitor *tm, char *buf, int bufsz)
{
    int pos = 0;
    pos += snprintf(buf + pos, bufsz - pos,
                    "=== Tunnel Monitor ===\nActive: %d / %d\nTotal bytes: %ld\n\n",
                    tm->total_active, tm->count, tm->total_bytes);
    static const char *states[] = {"IDLE","CONNECTING","ACTIVE","ERROR","RECONNECTING"};
    for (int i = 0; i < tm->count && pos < bufsz - 200; i++) {
        const TMTunnel *t = &tm->tunnels[i];
        pos += snprintf(buf + pos, bufsz - pos,
                       "[%s] %s:%d -> %s:%d  State: %s  RX: %ld  TX: %ld\n",
                       t->label, t->local_bind[0] ? t->local_bind : "127.0.0.1",
                       t->local_port, t->remote_host, t->remote_port,
                       states[t->state], t->bytes_rx, t->bytes_tx);
    }
    return pos;
}
