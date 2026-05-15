/*
 * puttyalt_quickconnect.c: Quick-connect URI parser implementation.
 */

#include "puttyalt_quickconnect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int quickconnect_default_port(QCProtocol proto)
{
    switch (proto) {
    case QC_PROTO_SSH:    return 22;
    case QC_PROTO_TELNET: return 23;
    case QC_PROTO_RAW:    return 0;
    case QC_PROTO_SERIAL: return 0;
    default:              return 0;
    }
}

/*
 * Parse a URI or shorthand connection string.
 */
int quickconnect_parse(const char *input, QuickConnectParams *out)
{
    const char *p = input;

    memset(out, 0, sizeof(*out));
    out->protocol = QC_PROTO_SSH;  /* default */

    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p))
        p++;

    if (!*p)
        return 0;

    /* Check for scheme:// prefix */
    const char *scheme_end = strstr(p, "://");
    if (scheme_end) {
        size_t slen = (size_t)(scheme_end - p);
        if (slen == 3 && strncmp(p, "ssh", 3) == 0)
            out->protocol = QC_PROTO_SSH;
        else if (slen == 6 && strncmp(p, "telnet", 6) == 0)
            out->protocol = QC_PROTO_TELNET;
        else if (slen == 3 && strncmp(p, "raw", 3) == 0)
            out->protocol = QC_PROTO_RAW;
        else if (slen == 6 && strncmp(p, "serial", 6) == 0)
            out->protocol = QC_PROTO_SERIAL;
        else
            out->protocol = QC_PROTO_UNKNOWN;

        p = scheme_end + 3;
    }

    /* Check for user@ */
    const char *at = strchr(p, '@');
    if (at) {
        size_t ulen = (size_t)(at - p);
        if (ulen >= sizeof(out->user))
            ulen = sizeof(out->user) - 1;
        memcpy(out->user, p, ulen);
        out->user[ulen] = '\0';
        p = at + 1;
    }

    /* Handle IPv6 addresses in brackets: [::1]:port */
    if (*p == '[') {
        const char *bracket = strchr(p, ']');
        if (!bracket)
            return 0;

        size_t hlen = (size_t)(bracket - p - 1);
        if (hlen >= sizeof(out->host))
            hlen = sizeof(out->host) - 1;
        memcpy(out->host, p + 1, hlen);
        out->host[hlen] = '\0';

        p = bracket + 1;
        if (*p == ':') {
            out->port = atoi(p + 1);
        }
    } else {
        /* Regular hostname, possibly with :port */
        const char *colon = strchr(p, ':');
        if (colon) {
            size_t hlen = (size_t)(colon - p);
            if (hlen >= sizeof(out->host))
                hlen = sizeof(out->host) - 1;
            memcpy(out->host, p, hlen);
            out->host[hlen] = '\0';
            out->port = atoi(colon + 1);
        } else {
            /* Trim trailing whitespace */
            size_t hlen = strlen(p);
            while (hlen > 0 && isspace((unsigned char)p[hlen - 1]))
                hlen--;
            if (hlen >= sizeof(out->host))
                hlen = sizeof(out->host) - 1;
            memcpy(out->host, p, hlen);
            out->host[hlen] = '\0';
        }
    }

    /* Validate */
    if (out->host[0] == '\0')
        return 0;

    if (out->port == 0)
        out->port = quickconnect_default_port(out->protocol);

    out->valid = 1;
    return 1;
}

int quickconnect_format(const QuickConnectParams *p, char *buf, int bufsize)
{
    const char *proto_str = "ssh";
    switch (p->protocol) {
    case QC_PROTO_SSH:    proto_str = "ssh"; break;
    case QC_PROTO_TELNET: proto_str = "telnet"; break;
    case QC_PROTO_RAW:    proto_str = "raw"; break;
    case QC_PROTO_SERIAL: proto_str = "serial"; break;
    default:              proto_str = "unknown"; break;
    }

    if (p->user[0]) {
        return snprintf(buf, bufsize, "%s://%s@%s:%d",
                        proto_str, p->user, p->host, p->port);
    } else {
        return snprintf(buf, bufsize, "%s://%s:%d",
                        proto_str, p->host, p->port);
    }
}
