#include <string.h>
#include <stdio.h>
#include <time.h>
typedef struct { int interval_sec; int max_missed; int missed_count; time_t last_sent; time_t last_recv; int enabled; } KeepaliveConfig;
static KeepaliveConfig g_ka = {30, 3, 0, 0, 0, 1};
void keepalive_configure(int interval, int max_missed) { g_ka.interval_sec = interval; g_ka.max_missed = max_missed; }
int keepalive_should_send(void) {
    if (!g_ka.enabled) return 0;
    time_t now = time(NULL);
    return (now - g_ka.last_sent) >= g_ka.interval_sec;
}
void keepalive_sent(void) { g_ka.last_sent = time(NULL); g_ka.missed_count++; }
void keepalive_recv(void) { g_ka.last_recv = time(NULL); g_ka.missed_count = 0; }
int keepalive_is_dead(void) { return g_ka.missed_count >= g_ka.max_missed; }
int keepalive_status(char *buf, int buflen) {
    return snprintf(buf, buflen, "KA: %ds interval, %d/%d missed", g_ka.interval_sec, g_ka.missed_count, g_ka.max_missed);
}
