/* puttyalt_proxyparse.c - Parse proxy URL specifications.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
typedef struct { char scheme[8]; char host[64]; int port; char user[32]; char pass[32]; } ProxySpec;
/* Parse "socks5://user:pass@host:1080". Returns 0 on success. */
int pxp_parse(const char *url, ProxySpec *out) {
    if (!url || !out) return -1;
    memset(out, 0, sizeof *out);
    const char *p = url;
    const char *sep = strstr(p, "://");
    if (sep) {
        int sl = (int)(sep - p); if (sl >= 8) sl = 7;
        memcpy(out->scheme, p, sl); out->scheme[sl] = 0;
        p = sep + 3;
    }
    const char *at = strchr(p, '@');
    if (at) {
        const char *colon = strchr(p, ':');
        if (colon && colon < at) {
            int ul = (int)(colon - p); if (ul >= 32) ul = 31;
            memcpy(out->user, p, ul); out->user[ul] = 0;
            int pl = (int)(at - colon - 1); if (pl >= 32) pl = 31;
            memcpy(out->pass, colon + 1, pl); out->pass[pl] = 0;
        } else {
            int ul = (int)(at - p); if (ul >= 32) ul = 31;
            memcpy(out->user, p, ul); out->user[ul] = 0;
        }
        p = at + 1;
    }
    const char *pc = strchr(p, ':');
    if (pc) {
        int hl = (int)(pc - p); if (hl >= 64) hl = 63;
        memcpy(out->host, p, hl); out->host[hl] = 0;
        out->port = atoi(pc + 1);
    } else {
        strncpy(out->host, p, 63); out->host[63] = 0;
        out->port = 1080;
    }
    return out->host[0] ? 0 : -1;
}
