#ifndef PUTTYALT_NETSCAN_H
#define PUTTYALT_NETSCAN_H

#define NS_MAX_HOSTS    256
#define NS_MAX_PORTS    64
#define NS_MAX_HOST     256

typedef enum {
    NS_PORT_CLOSED = 0,
    NS_PORT_OPEN,
    NS_PORT_FILTERED,
    NS_PORT_UNKNOWN
} NSPortState;

typedef struct {
    int         port;
    NSPortState state;
    char        service[32];
    int         latency_ms;
} NSPort;

typedef struct {
    char    hostname[NS_MAX_HOST];
    char    ip[46];
    NSPort  ports[NS_MAX_PORTS];
    int     port_count;
    int     alive;
    int     latency_ms;
    long    scan_time;
} NSHost;

typedef struct {
    NSHost hosts[NS_MAX_HOSTS];
    int    count;
    int    scanning;
    int    progress;
    int    timeout_ms;
    char   range[NS_MAX_HOST];
} NetScanner;

void netscan_init(NetScanner *ns);
int  netscan_add_host(NetScanner *ns, const char *hostname);
int  netscan_add_range(NetScanner *ns, const char *cidr);
int  netscan_scan_port(NetScanner *ns, int host_idx, int port);
int  netscan_quick_scan(NetScanner *ns, int host_idx);
void netscan_clear(NetScanner *ns);
int  netscan_export(const NetScanner *ns, const char *path);
const char *netscan_port_service(int port);

#endif
