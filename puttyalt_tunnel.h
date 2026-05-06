#ifndef PUTTYALT_TUNNEL_H
#define PUTTYALT_TUNNEL_H

#define TUNNEL_MAX       32
#define TUNNEL_HOST_LEN  256

typedef enum {
    TUNNEL_LOCAL = 0,
    TUNNEL_REMOTE,
    TUNNEL_DYNAMIC
} TunnelType;

typedef enum {
    TUNNEL_STOPPED = 0,
    TUNNEL_STARTING,
    TUNNEL_ACTIVE,
    TUNNEL_ERROR
} TunnelState;

typedef struct SSHTunnel {
    TunnelType type;
    TunnelState state;
    int local_port;
    char remote_host[TUNNEL_HOST_LEN];
    int remote_port;
    int bind_loopback;       /* bind to 127.0.0.1 only */
    unsigned long bytes_fwd; /* bytes forwarded */
    int auto_reconnect;
} SSHTunnel;

typedef struct TunnelManager {
    SSHTunnel tunnels[TUNNEL_MAX];
    int count;
} TunnelManager;

void tunnel_init(TunnelManager *tm);
int  tunnel_add_local(TunnelManager *tm, int local_port,
                      const char *rhost, int rport);
int  tunnel_add_remote(TunnelManager *tm, int remote_port,
                       const char *lhost, int lport);
int  tunnel_add_dynamic(TunnelManager *tm, int socks_port);
int  tunnel_remove(TunnelManager *tm, int index);
int  tunnel_start(TunnelManager *tm, int index);
int  tunnel_stop(TunnelManager *tm, int index);
void tunnel_update_stats(TunnelManager *tm, int index, unsigned long bytes);

#endif
