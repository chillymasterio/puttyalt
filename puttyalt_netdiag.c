#include <string.h>
#include <stdio.h>
#include <time.h>
#include "puttyalt_netdiag.h"

#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif

int diag_resolve(DNSResult *result, const char *hostname)
{
    memset(result, 0, sizeof(*result));
    strncpy(result->hostname, hostname, DIAG_HOST_LEN - 1);

#ifndef _WIN32
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(hostname, NULL, &hints, &res);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    result->resolve_ms = (t1.tv_sec - t0.tv_sec) * 1000.0 +
                         (t1.tv_nsec - t0.tv_nsec) / 1e6;

    if (rc != 0) return -1;

    for (rp = res; rp && result->num_addresses < 8; rp = rp->ai_next) {
        char buf[64];
        void *addr;
        if (rp->ai_family == AF_INET) {
            addr = &((struct sockaddr_in *)rp->ai_addr)->sin_addr;
        } else if (rp->ai_family == AF_INET6) {
            addr = &((struct sockaddr_in6 *)rp->ai_addr)->sin6_addr;
            result->is_ipv6 = 1;
        } else continue;

        inet_ntop(rp->ai_family, addr, buf, sizeof(buf));
        strncpy(result->addresses[result->num_addresses], buf, 63);
        result->num_addresses++;
    }
    freeaddrinfo(res);
#endif
    return 0;
}

int diag_tcp_connect_time(const char *host, int port, double *ms)
{
#ifndef _WIN32
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char port_str[8];
    snprintf(port_str, sizeof(port_str), "%d", port);
    if (getaddrinfo(host, port_str, &hints, &res) != 0) return -1;

    int fd = socket(res->ai_family, SOCK_STREAM, 0);
    if (fd < 0) { freeaddrinfo(res); return -1; }

    /* Non-blocking connect */
    fcntl(fd, F_SETFL, O_NONBLOCK);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    connect(fd, res->ai_addr, res->ai_addrlen);

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(fd, &wfds);
    struct timeval tv = {5, 0};
    int sel = select(fd + 1, NULL, &wfds, NULL, &tv);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    *ms = (t1.tv_sec - t0.tv_sec) * 1000.0 +
          (t1.tv_nsec - t0.tv_nsec) / 1e6;

    close(fd);
    freeaddrinfo(res);
    return sel > 0 ? 0 : -1;
#else
    (void)host; (void)port; (void)ms;
    return -1;
#endif
}

int diag_estimate_mtu(const char *host)
{
    (void)host;
    return 1500; /* Default; real impl would use path MTU discovery */
}

void diag_format_report(const NetDiagnostics *diag, char *buf, int buflen)
{
    int n = 0;
    n += snprintf(buf + n, buflen - n, "=== Network Diagnostics ===\n\n");
    n += snprintf(buf + n, buflen - n, "DNS Resolution: %.1f ms\n",
                  diag->dns.resolve_ms);
    for (int i = 0; i < diag->dns.num_addresses && n < buflen; i++)
        n += snprintf(buf + n, buflen - n, "  %s\n",
                      diag->dns.addresses[i]);
    n += snprintf(buf + n, buflen - n, "\nTCP Connect: %.1f ms\n",
                  diag->tcp_connect_ms);
    n += snprintf(buf + n, buflen - n, "SSH Handshake: %.1f ms\n",
                  diag->ssh_handshake_ms);
    n += snprintf(buf + n, buflen - n, "Estimated MTU: %d\n", diag->mtu);
    (void)n;
}
