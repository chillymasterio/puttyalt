/*
 * puttyalt_proxydetect.c: Proxy detection implementation.
 */

#include "puttyalt_proxydetect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#endif

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
        /* Wildcard suffix match: *.example.com */
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

#ifdef _WIN32
int proxy_detect_system(ProxyConfig *pc)
{
    HKEY key;
    LONG ret;

    memset(pc, 0, sizeof(*pc));

    ret = RegOpenKeyExA(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
        0, KEY_READ, &key);
    if (ret != ERROR_SUCCESS)
        return -1;

    DWORD enabled = 0;
    DWORD dlen = sizeof(enabled);
    RegQueryValueExA(key, "ProxyEnable", NULL, NULL, (LPBYTE)&enabled, &dlen);

    if (enabled) {
        char server[512] = {0};
        DWORD slen = sizeof(server);
        RegQueryValueExA(key, "ProxyServer", NULL, NULL, (LPBYTE)server, &slen);

        if (server[0])
            proxy_parse_url(server, pc);

        char bypass[1024] = {0};
        DWORD blen = sizeof(bypass);
        RegQueryValueExA(key, "ProxyOverride", NULL, NULL, (LPBYTE)bypass, &blen);
        if (bypass[0])
            snprintf(pc->bypass, sizeof(pc->bypass), "%s", bypass);

        pc->auto_detected = 1;
    }

    RegCloseKey(key);
    return enabled ? 0 : -1;
}
#else
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
#endif

/* PAC (Proxy Auto-Config) file detection stub */
int proxy_detect_pac(const char *pac_url, char *proxy_out, int outlen)
{
    if (!pac_url || !proxy_out) return -1;
    if (strstr(pac_url, ".pac") || strstr(pac_url, "wpad.dat")) {
        snprintf(proxy_out, outlen, "PAC:%s", pac_url);
        return 1;
    }
    return 0;
}
