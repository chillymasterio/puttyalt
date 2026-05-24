#include "puttyalt_netscan.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void netscan_init(NetScanner *ns)
{
    memset(ns, 0, sizeof(*ns));
    ns->timeout_ms = 3000;
}

int netscan_add_host(NetScanner *ns, const char *hostname)
{
    if (ns->count >= NS_MAX_HOSTS) return -1;
    NSHost *h = &ns->hosts[ns->count];
    memset(h, 0, sizeof(*h));
    snprintf(h->hostname, NS_MAX_HOST, "%s", hostname);
    return ns->count++;
}

int netscan_add_range(NetScanner *ns, const char *cidr)
{
    /* Parse CIDR and add individual hosts */
    snprintf(ns->range, NS_MAX_HOST, "%s", cidr);
    char base[46];
    int prefix = 24;
    snprintf(base, sizeof(base), "%s", cidr);
    char *slash = strchr(base, '/');
    if (slash) { *slash = '\0'; prefix = atoi(slash + 1); }

    /* For simplicity, handle /24 only */
    if (prefix != 24) return netscan_add_host(ns, base);

    char *last_dot = strrchr(base, '.');
    if (!last_dot) return -1;
    *last_dot = '\0';

    int added = 0;
    for (int i = 1; i <= 254 && ns->count < NS_MAX_HOSTS; i++) {
        char host[46];
        snprintf(host, sizeof(host), "%s.%d", base, i);
        netscan_add_host(ns, host);
        added++;
    }
    return added;
}

const char *netscan_port_service(int port)
{
    switch (port) {
    case 21: return "FTP";
    case 22: return "SSH";
    case 23: return "Telnet";
    case 25: return "SMTP";
    case 53: return "DNS";
    case 80: return "HTTP";
    case 110: return "POP3";
    case 143: return "IMAP";
    case 443: return "HTTPS";
    case 993: return "IMAPS";
    case 995: return "POP3S";
    case 3306: return "MySQL";
    case 3389: return "RDP";
    case 5432: return "PostgreSQL";
    case 5900: return "VNC";
    case 6379: return "Redis";
    case 8080: return "HTTP-Alt";
    case 8443: return "HTTPS-Alt";
    case 27017: return "MongoDB";
    default: return "unknown";
    }
}

int netscan_scan_port(NetScanner *ns, int host_idx, int port)
{
    if (host_idx < 0 || host_idx >= ns->count) return -1;
    NSHost *h = &ns->hosts[host_idx];
    if (h->port_count >= NS_MAX_PORTS) return -1;

    NSPort *p = &h->ports[h->port_count];
    p->port = port;
    p->state = NS_PORT_UNKNOWN;
    snprintf(p->service, sizeof(p->service), "%s", netscan_port_service(port));

    /* Actual scanning would use connect() with timeout.
       Stubbed for cross-platform compilation. */
    p->state = NS_PORT_UNKNOWN;
    p->latency_ms = 0;
    h->port_count++;
    return 0;
}

int netscan_quick_scan(NetScanner *ns, int host_idx)
{
    static const int common[] = {22, 23, 80, 443, 3389, 5900, 8080};
    int n = sizeof(common) / sizeof(common[0]);
    for (int i = 0; i < n; i++)
        netscan_scan_port(ns, host_idx, common[i]);
    return n;
}

void netscan_clear(NetScanner *ns)
{
    ns->count = 0;
    ns->scanning = 0;
    ns->progress = 0;
}

int netscan_export(const NetScanner *ns, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "# Network Scan Report\n# Hosts: %d\n\n", ns->count);
    for (int i = 0; i < ns->count; i++) {
        const NSHost *h = &ns->hosts[i];
        fprintf(f, "Host: %s", h->hostname);
        if (h->ip[0]) fprintf(f, " (%s)", h->ip);
        fprintf(f, " - %s\n", h->alive ? "UP" : "DOWN/UNKNOWN");
        for (int j = 0; j < h->port_count; j++) {
            const NSPort *p = &h->ports[j];
            const char *state = "?";
            if (p->state == NS_PORT_OPEN) state = "OPEN";
            else if (p->state == NS_PORT_CLOSED) state = "CLOSED";
            else if (p->state == NS_PORT_FILTERED) state = "FILTERED";
            fprintf(f, "  %5d/tcp  %-10s  %s\n", p->port, state, p->service);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
