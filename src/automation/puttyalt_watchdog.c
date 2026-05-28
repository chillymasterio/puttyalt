#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_WATCHDOGS 16
typedef struct { char name[64]; char pattern[256]; time_t last_seen; int timeout_sec; int triggered; void (*on_timeout)(const char*); } Watchdog;
static Watchdog g_wd[MAX_WATCHDOGS]; static int g_wd_count = 0;
int watchdog_add(const char *name, const char *pattern, int timeout_sec) {
    if (g_wd_count >= MAX_WATCHDOGS) return -1;
    Watchdog *w = &g_wd[g_wd_count]; memset(w, 0, sizeof(*w));
    snprintf(w->name, 64, "%s", name); snprintf(w->pattern, 256, "%s", pattern);
    w->timeout_sec = timeout_sec; w->last_seen = time(NULL); return g_wd_count++;
}
void watchdog_feed(const char *output) {
    for (int i=0;i<g_wd_count;i++) if (strstr(output, g_wd[i].pattern)) { g_wd[i].last_seen = time(NULL); g_wd[i].triggered = 0; }
}
int watchdog_check(void) {
    time_t now = time(NULL); int triggered = 0;
    for (int i=0;i<g_wd_count;i++) if (!g_wd[i].triggered && (now - g_wd[i].last_seen) > g_wd[i].timeout_sec) { g_wd[i].triggered = 1; triggered++; }
    return triggered;
}
int watchdog_count(void) { return g_wd_count; }
