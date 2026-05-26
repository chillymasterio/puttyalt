#include <string.h>
#include <stdio.h>
#include <time.h>
typedef struct { int enabled; char format[64]; int utc; } TimestampConfig;
static TimestampConfig g_ts = {1, "[%H:%M:%S]", 0};
void timestamp_set_format(const char *fmt) { snprintf(g_ts.format, 64, "%s", fmt); }
void timestamp_set_utc(int utc) { g_ts.utc = utc; }
void timestamp_enable(int enabled) { g_ts.enabled = enabled; }
int timestamp_format(char *buf, int buflen) {
    if (!g_ts.enabled) { buf[0] = 0; return 0; }
    time_t now = time(NULL);
    struct tm *t = g_ts.utc ? gmtime(&now) : localtime(&now);
    return (int)strftime(buf, buflen, g_ts.format, t);
}
