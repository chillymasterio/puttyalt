#include <string.h>
#include "puttyalt_tunnel.h"

void tunnel_init(TunnelManager *tm)
{
    memset(tm, 0, sizeof(*tm));
}

int tunnel_add_local(TunnelManager *tm, int local_port,
                     const char *rhost, int rport)
{
    if (tm->count >= TUNNEL_MAX) return -1;
    SSHTunnel *t = &tm->tunnels[tm->count];
    memset(t, 0, sizeof(*t));
    t->type = TUNNEL_LOCAL;
    t->local_port = local_port;
    strncpy(t->remote_host, rhost, TUNNEL_HOST_LEN - 1);
    t->remote_port = rport;
    t->bind_loopback = 1;
    t->state = TUNNEL_STOPPED;
    tm->count++;
    return tm->count - 1;
}

int tunnel_add_remote(TunnelManager *tm, int remote_port,
                      const char *lhost, int lport)
{
    if (tm->count >= TUNNEL_MAX) return -1;
    SSHTunnel *t = &tm->tunnels[tm->count];
    memset(t, 0, sizeof(*t));
    t->type = TUNNEL_REMOTE;
    t->remote_port = remote_port;
    strncpy(t->remote_host, lhost, TUNNEL_HOST_LEN - 1);
    t->local_port = lport;
    t->bind_loopback = 1;
    t->state = TUNNEL_STOPPED;
    tm->count++;
    return tm->count - 1;
}

int tunnel_add_dynamic(TunnelManager *tm, int socks_port)
{
    if (tm->count >= TUNNEL_MAX) return -1;
    SSHTunnel *t = &tm->tunnels[tm->count];
    memset(t, 0, sizeof(*t));
    t->type = TUNNEL_DYNAMIC;
    t->local_port = socks_port;
    t->bind_loopback = 1;
    t->state = TUNNEL_STOPPED;
    tm->count++;
    return tm->count - 1;
}

int tunnel_remove(TunnelManager *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    if (tm->tunnels[index].state == TUNNEL_ACTIVE) return -1;
    for (int i = index; i < tm->count - 1; i++)
        tm->tunnels[i] = tm->tunnels[i + 1];
    tm->count--;
    return 0;
}

int tunnel_start(TunnelManager *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    tm->tunnels[index].state = TUNNEL_STARTING;
    /* Actual socket binding happens in the network layer */
    return 0;
}

int tunnel_stop(TunnelManager *tm, int index)
{
    if (index < 0 || index >= tm->count) return -1;
    tm->tunnels[index].state = TUNNEL_STOPPED;
    return 0;
}

void tunnel_update_stats(TunnelManager *tm, int index, unsigned long bytes)
{
    if (index < 0 || index >= tm->count) return;
    tm->tunnels[index].bytes_fwd += bytes;
}
