#include <string.h>
#include <stdio.h>
typedef struct { char filename[256]; long long total_bytes; long long transferred; int pct; double speed_kbps; int eta_seconds; } ScpProgress;
static ScpProgress g_scp; static int g_active = 0;
void scp_progress_start(const char *filename, long long total) {
    memset(&g_scp, 0, sizeof(g_scp)); snprintf(g_scp.filename, 256, "%s", filename);
    g_scp.total_bytes = total; g_active = 1;
}
void scp_progress_update(long long transferred, double speed) {
    g_scp.transferred = transferred; g_scp.speed_kbps = speed;
    g_scp.pct = g_scp.total_bytes > 0 ? (int)(transferred * 100 / g_scp.total_bytes) : 0;
    g_scp.eta_seconds = speed > 0 ? (int)((g_scp.total_bytes - transferred) / (speed * 1024)) : 0;
}
void scp_progress_done(void) { g_scp.pct = 100; g_active = 0; }
int scp_progress_format(char *buf, int buflen) {
    if (!g_active) return 0;
    return snprintf(buf, buflen, "%s: %d%% (%.1f KB/s, ETA %ds)", g_scp.filename, g_scp.pct, g_scp.speed_kbps, g_scp.eta_seconds);
}
int scp_progress_is_active(void) { return g_active; }
