#include <string.h>
#include <stdio.h>
#define MAX_FWD 64
typedef enum { FWD_LOCAL, FWD_REMOTE, FWD_DYNAMIC } FwdType;
typedef struct { FwdType type; int local_port; char remote_host[256]; int remote_port; int active; char description[128]; } PortForward;
static PortForward g_fwds[MAX_FWD]; static int g_fwd_count = 0;
int portfwd_add(FwdType type, int local_port, const char *remote_host, int remote_port, const char *desc) {
    if (g_fwd_count >= MAX_FWD) return -1;
    PortForward *f = &g_fwds[g_fwd_count]; memset(f, 0, sizeof(*f));
    f->type = type; f->local_port = local_port; f->remote_port = remote_port; f->active = 1;
    snprintf(f->remote_host, 256, "%s", remote_host);
    if (desc) snprintf(f->description, 128, "%s", desc);
    return g_fwd_count++;
}
int portfwd_remove(int idx) {
    if (idx < 0 || idx >= g_fwd_count) return -1;
    memmove(&g_fwds[idx], &g_fwds[idx+1], sizeof(PortForward)*(g_fwd_count-idx-1));
    g_fwd_count--; return 0;
}
int portfwd_toggle(int idx) {
    if (idx < 0 || idx >= g_fwd_count) return -1;
    g_fwds[idx].active = !g_fwds[idx].active; return g_fwds[idx].active;
}
int portfwd_format(int idx, char *buf, int buflen) {
    if (idx < 0 || idx >= g_fwd_count) return -1;
    PortForward *f = &g_fwds[idx];
    const char *types[] = {"Local", "Remote", "Dynamic"};
    return snprintf(buf, buflen, "[%s] %s:%d -> %s:%d %s", f->active?"ON":"OFF",
        types[f->type], f->local_port, f->remote_host, f->remote_port, f->description);
}
int portfwd_count(void) { return g_fwd_count; }
