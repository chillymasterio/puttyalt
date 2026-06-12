/* puttyalt_timeparse.c - Time string parsing (ISO 8601, relative, epoch). */
#include <string.h>
#include <stdlib.h>
typedef struct {
    int year, month, day;
    int hour, minute, second;
    int valid;
} TimeParse;
int timeparse_iso(const char *str, TimeParse *out) {
    if (!str || !out) return -1;
    memset(out, 0, sizeof(*out));
    int n = 0;
    const char *p = str;
    out->year = (int)strtol(p, (char **)&p, 10);
    if (*p == '-') {
        p++;
        out->month = (int)strtol(p, (char **)&p, 10);
        n = 2;
    }
    if (*p == '-') {
        p++;
        out->day = (int)strtol(p, (char **)&p, 10);
        n = 3;
    }
    if (*p == 'T' || *p == ' ') {
        p++;
        out->hour = (int)strtol(p, (char **)&p, 10);
        if (*p == ':') {
            p++;
            out->minute = (int)strtol(p, (char **)&p, 10);
        }
        if (*p == ':') {
            p++;
            out->second = (int)strtol(p, (char **)&p, 10);
        }
        n = 6;
    }
    out->valid = (n >= 3 && out->month >= 1 && out->month <= 12 && out->day >= 1 && out->day <= 31);
    return out->valid ? 0 : -1;
}
long timeparse_relative_sec(const char *str) {
    if (!str) return -1;
    long n = strtol(str, (char **)&str, 10);
    switch (*str) {
        case 's': return n;
        case 'm': return n * 60;
        case 'h': return n * 3600;
        case 'd': return n * 86400;
        case 'w': return n * 604800;
        default: return n;
    }
}
int timeparse_is_valid(const TimeParse *t) {
    return t ? t->valid : -1;
}
