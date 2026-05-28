#include <string.h>
#include <stdio.h>
int uptime_format(long seconds, char *buf, int buflen) {
    int d = (int)(seconds / 86400); int h = (int)((seconds % 86400) / 3600);
    int m = (int)((seconds % 3600) / 60); int s = (int)(seconds % 60);
    if (d > 0) return snprintf(buf, buflen, "%dd %dh %dm %ds", d, h, m, s);
    if (h > 0) return snprintf(buf, buflen, "%dh %dm %ds", h, m, s);
    if (m > 0) return snprintf(buf, buflen, "%dm %ds", m, s);
    return snprintf(buf, buflen, "%ds", s);
}
int uptime_format_short(long seconds, char *buf, int buflen) {
    if (seconds >= 86400) return snprintf(buf, buflen, "%ldd", seconds / 86400);
    if (seconds >= 3600) return snprintf(buf, buflen, "%ldh", seconds / 3600);
    if (seconds >= 60) return snprintf(buf, buflen, "%ldm", seconds / 60);
    return snprintf(buf, buflen, "%lds", seconds);
}
