#ifndef PUTTYALT_NETDIAG_H
#define PUTTYALT_NETDIAG_H

#define DIAG_MAX_HOPS     30
#define DIAG_HOST_LEN     256

typedef struct DiagHop {
    char host[DIAG_HOST_LEN];
    char ip[64];
    double rtt_ms[3];    /* 3 probes */
    int timeout_mask;    /* bit per probe: 1 = timeout */
} DiagHop;

typedef struct TracerouteResult {
    DiagHop hops[DIAG_MAX_HOPS];
    int num_hops;
    int complete;
} TracerouteResult;

typedef struct DNSResult {
    char hostname[DIAG_HOST_LEN];
    char addresses[8][64];
    int num_addresses;
    double resolve_ms;
    int is_ipv6;
} DNSResult;

typedef struct NetDiagnostics {
    TracerouteResult trace;
    DNSResult dns;
    double tcp_connect_ms;
    double ssh_handshake_ms;
    int mtu;
} NetDiagnostics;

int  diag_resolve(DNSResult *result, const char *hostname);
int  diag_tcp_connect_time(const char *host, int port, double *ms);
int  diag_estimate_mtu(const char *host);
void diag_format_report(const NetDiagnostics *diag, char *buf, int buflen);

#endif
