/*
 * puttyalt_proxydetect.h: System proxy auto-detection.
 *
 * Reads proxy settings from the OS (Windows registry / environment)
 * and applies them to SSH connections automatically.
 */

#ifndef PUTTYALT_PROXYDETECT_H
#define PUTTYALT_PROXYDETECT_H

typedef enum {
    PROXY_NONE,
    PROXY_HTTP,
    PROXY_SOCKS4,
    PROXY_SOCKS5
} ProxyType;

typedef struct ProxyConfig {
    ProxyType type;
    char      host[256];
    int       port;
    char      username[128];
    char      password[128];
    char      bypass[1024];    /* comma-separated bypass list */
    int       auto_detected;
} ProxyConfig;

/* Detect system proxy settings */
int proxy_detect_system(ProxyConfig *pc);

/* Check if a hostname should bypass the proxy */
int proxy_should_bypass(const ProxyConfig *pc, const char *hostname);

/* Parse proxy URL like http://user:pass@host:port */
int proxy_parse_url(const char *url, ProxyConfig *pc);

#endif /* PUTTYALT_PROXYDETECT_H */
