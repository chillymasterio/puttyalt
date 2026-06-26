/* puttyalt_tunnelmap.c - Map local/remote port forwards.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
#define TN_MAX 16
typedef struct { int local_port; char remote_host[64]; int remote_port; int reverse; } TnForward;
typedef struct { TnForward f[TN_MAX]; int n; } TunnelMap;
void tn2_init(TunnelMap *t) { if (t) t->n = 0; }
int tn2_add(TunnelMap *t, int local_port, const char *remote_host, int remote_port, int reverse) {
    if (!t || t->n >= TN_MAX || !remote_host) return -1;
    t->f[t->n].local_port = local_port;
    strncpy(t->f[t->n].remote_host, remote_host, 63); t->f[t->n].remote_host[63] = 0;
    t->f[t->n].remote_port = remote_port; t->f[t->n].reverse = reverse;
    t->n++;
    return 0;
}
int tn2_find_local(const TunnelMap *t, int local_port) {
    if (!t) return -1;
    for (int i = 0; i < t->n; i++) if (t->f[i].local_port == local_port && !t->f[i].reverse) return i;
    return -1;
}
int tn2_format(const TnForward *f, char *out, int outlen) {
    if (!f || !out) return -1;
    return snprintf(out, outlen, "%s%d:%s:%d",
                    f->reverse ? "R" : "L", f->local_port, f->remote_host, f->remote_port);
}
