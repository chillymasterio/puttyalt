/* puttyalt_proxyconfig.c - Parse proxy environment configuration.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
typedef struct { char host[64]; int port; int enabled; } ProxyConfig;
/* Parse "http://host:port" style proxy env value. */
int pc4_parse(const char *value, ProxyConfig *out) {
    if (!value || !out) return -1;
    out->enabled = 0; out->port = 8080; out->host[0] = 0;
    const char *p = strstr(value, "://");
    p = p ? p + 3 : value;
    const char *colon = strrchr(p, ':');
    if (colon) {
        int hl = (int)(colon - p); if (hl >= 64) hl = 63;
        memcpy(out->host, p, hl); out->host[hl] = 0;
        out->port = atoi(colon + 1);
    } else {
        strncpy(out->host, p, 63); out->host[63] = 0;
    }
    out->enabled = out->host[0] ? 1 : 0;
    return out->enabled ? 0 : -1;
}
/* Check if a host matches a no_proxy list. */
int pc4_no_proxy(const char *no_proxy, const char *host) {
    if (!no_proxy || !host) return 0;
    char buf[256]; strncpy(buf, no_proxy, 255); buf[255] = 0;
    char *tok = strtok(buf, ",");
    while (tok) {
        while (*tok == ' ') tok++;
        if (strstr(host, tok)) return 1;
        tok = strtok(NULL, ",");
    }
    return 0;
}
