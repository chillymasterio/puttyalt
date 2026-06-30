/* puttyalt_hostport.c - Split and join host:port strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int hp2_split(const char *s, char *host, int hostlen, int *port) {
    if (!s || !host || !port) return -1;
    /* [ipv6]:port form */
    if (*s == '[') {
        const char *end = strchr(s, ']');
        if (!end) return -1;
        int hl = (int)(end - s - 1);
        if (hl >= hostlen) hl = hostlen - 1;
        memcpy(host, s + 1, hl);
        host[hl] = 0;
        *port = (end[1] == ':') ? atoi(end + 2) : 0;
        return 0;
    }
    /* Only treat as host:port when there is exactly one colon (a bare IPv6
       address has several and must be bracketed to carry a port). */
    const char *first = strchr(s, ':');
    const char *last = strrchr(s, ':');
    if (first && first == last) {
        int hl = (int)(first - s);
        if (hl >= hostlen) hl = hostlen - 1;
        memcpy(host, s, hl);
        host[hl] = 0;
        *port = atoi(first + 1);
    } else {
        strncpy(host, s, hostlen - 1);
        host[hostlen - 1] = 0;
        *port = 0;
    }
    return 0;
}

int hp2_join(const char *host, int port, char *out, int outlen) {
    if (!host || !out) return -1;
    int is_v6 = strchr(host, ':') != 0;
    if (port > 0)
        return snprintf(out, outlen, is_v6 ? "[%s]:%d" : "%s:%d", host, port);
    int n = (int)strlen(host);
    if (n >= outlen) return -1;
    memcpy(out, host, n);
    out[n] = 0;
    return n;
}
