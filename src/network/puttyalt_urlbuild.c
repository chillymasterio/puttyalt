/* puttyalt_urlbuild.c - Build URLs from components.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { char scheme[8]; char host[96]; int port; char path[128]; } UrlParts;
int ub2_build(const UrlParts *u, char *out, int outlen) {
    if (!u || !out) return -1;
    int o = snprintf(out, outlen, "%s://%s", u->scheme[0] ? u->scheme : "http", u->host);
    int default_port = (strcmp(u->scheme, "https") == 0) ? 443 : 80;
    if (u->port && u->port != default_port) o += snprintf(out + o, outlen - o, ":%d", u->port);
    if (u->path[0]) o += snprintf(out + o, outlen - o, "%s%s", u->path[0] == '/' ? "" : "/", u->path);
    return o;
}
int ub2_with_query(const char *base, const char *key, const char *val, char *out, int outlen) {
    if (!base || !key || !out) return -1;
    int has_q = strchr(base, '?') != 0;
    return snprintf(out, outlen, "%s%c%s=%s", base, has_q ? '&' : '?', key, val ? val : "");
}
