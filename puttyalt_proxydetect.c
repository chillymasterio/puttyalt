/*
 * puttyalt_proxydetect.c: Proxy detection via environment variables.
 */

#include "puttyalt_proxydetect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int proxy_parse_url(const char *url, ProxyConfig *pc)
{
    if (!url || !*url) return -1;

    memset(pc, 0, sizeof(*pc));

    const char *p = url;

    /* Detect scheme */
    if (strncmp(p, "http://", 7) == 0) {
        pc->type = PROXY_HTTP;
        p += 7;
    } else if (strncmp(p, "socks5://", 9) == 0) {
        pc->type = PROXY_SOCKS5;
        p += 9;
    } else if (strncmp(p, "socks4://", 9) == 0) {
        pc->type = PROXY_SOCKS4;
        p += 9;
    } else {
        pc->type = PROXY_HTTP;
    }

    /* Check for user:pass@ */
    const char *at = strchr(p, '@');
    if (at) {
        const char *colon = strchr(p, ':');
        if (colon && colon < at) {
            size_t ulen = (size_t)(colon - p);
            if (ulen >= sizeof(pc->username)) ulen = sizeof(pc->username) - 1;
            memcpy(pc->username, p, ulen);

            size_t plen = (size_t)(at - colon - 1);
            if (plen >= sizeof(pc->password)) plen = sizeof(pc->password) - 1;
            memcpy(pc->password, colon + 1, plen);
        }
        p = at + 1;
    }

    /* host:port */
    const char *colon = strchr(p, ':');
    if (colon) {
        size_t hlen = (size_t)(colon - p);
        if (hlen >= sizeof(pc->host)) hlen = sizeof(pc->host) - 1;
        memcpy(pc->host, p, hlen);
        pc->port = atoi(colon + 1);
    } else {
        snprintf(pc->host, sizeof(pc->host), "%s", p);
        pc->port = (pc->type == PROXY_HTTP) ? 8080 : 1080;
    }

    return 0;
}

int proxy_should_bypass(const ProxyConfig *pc, const char *hostname)
{
    if (!pc->bypass[0] || !hostname)
        return 0;

    char bypass_copy[1024];
    snprintf(bypass_copy, sizeof(bypass_copy), "%s", pc->bypass);

    char *token = strtok(bypass_copy, ",;");
    while (token) {
        while (*token == ' ') token++;
        if (strcmp(token, hostname) == 0)
            return 1;
        if (token[0] == '*' && token[1] == '.') {
            size_t slen = strlen(token + 1);
            size_t hlen = strlen(hostname);
            if (hlen >= slen && strcmp(hostname + hlen - slen, token + 1) == 0)
                return 1;
        }
        token = strtok(NULL, ",;");
    }
    return 0;
}

/* Cross-platform proxy detection via environment variables */
int proxy_detect_system(ProxyConfig *pc)
{
    memset(pc, 0, sizeof(*pc));

    const char *http_proxy = getenv("http_proxy");
    if (!http_proxy)
        http_proxy = getenv("HTTP_PROXY");
    if (!http_proxy)
        http_proxy = getenv("https_proxy");
    if (!http_proxy)
        http_proxy = getenv("HTTPS_PROXY");

    if (http_proxy && *http_proxy) {
        proxy_parse_url(http_proxy, pc);
        pc->auto_detected = 1;

        const char *no_proxy = getenv("no_proxy");
        if (!no_proxy) no_proxy = getenv("NO_PROXY");
        if (no_proxy)
            snprintf(pc->bypass, sizeof(pc->bypass), "%s", no_proxy);

        return 0;
    }

    return -1;
}

int proxy_detect_pac(const char *pac_url, char *proxy_out, int outlen)
{
    if (!pac_url || !proxy_out) return -1;
    if (strstr(pac_url, ".pac") || strstr(pac_url, "wpad.dat")) {
        snprintf(proxy_out, outlen, "PAC:%s", pac_url);
        return 1;
    }
    return 0;
}
