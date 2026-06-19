/* puttyalt_quickconnect.c - Parse quick-connect connection strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
typedef struct { char user[48]; char host[96]; int port; } QuickConn;
/* Parse "user@host:port" (user and port optional). */
int qc2_parse(const char *s, QuickConn *out) {
    if (!s || !out) return -1;
    out->user[0] = 0; out->host[0] = 0; out->port = 22;
    const char *at = strchr(s, '@');
    const char *hoststart = s;
    if (at) {
        int ul = (int)(at - s); if (ul >= 48) ul = 47;
        memcpy(out->user, s, ul); out->user[ul] = 0;
        hoststart = at + 1;
    }
    const char *colon = strchr(hoststart, ':');
    if (colon) {
        int hl = (int)(colon - hoststart); if (hl >= 96) hl = 95;
        memcpy(out->host, hoststart, hl); out->host[hl] = 0;
        out->port = atoi(colon + 1);
        if (out->port <= 0 || out->port > 65535) out->port = 22;
    } else {
        strncpy(out->host, hoststart, 95); out->host[95] = 0;
    }
    return out->host[0] ? 0 : -1;
}
