#include <string.h>
#include <stdio.h>
#include <time.h>

#define HISTORY_MAX 512

typedef struct {
    char host[256];
    int port;
    time_t connected_at;
    time_t disconnected_at;
    int exit_code;
    char disconnect_reason[128];
} ConnHistory;

static ConnHistory g_history[HISTORY_MAX];
static int g_history_count = 0;

int connhistory_add(const char *host, int port)
{
    if (g_history_count >= HISTORY_MAX) {
        memmove(&g_history[0], &g_history[1], sizeof(ConnHistory) * (HISTORY_MAX - 1));
        g_history_count = HISTORY_MAX - 1;
    }
    ConnHistory *h = &g_history[g_history_count];
    memset(h, 0, sizeof(*h));
    snprintf(h->host, 256, "%s", host);
    h->port = port;
    h->connected_at = time(NULL);
    return g_history_count++;
}

void connhistory_close(int idx, int exit_code, const char *reason)
{
    if (idx < 0 || idx >= g_history_count) return;
    g_history[idx].disconnected_at = time(NULL);
    g_history[idx].exit_code = exit_code;
    if (reason) snprintf(g_history[idx].disconnect_reason, 128, "%s", reason);
}

int connhistory_recent(ConnHistory **out, int max)
{
    int start = g_history_count > max ? g_history_count - max : 0;
    int n = g_history_count - start;
    for (int i = 0; i < n; i++) out[i] = &g_history[start + n - 1 - i];
    return n;
}

int connhistory_count(void) { return g_history_count; }
