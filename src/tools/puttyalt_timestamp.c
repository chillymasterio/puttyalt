#include "puttyalt_timestamp.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

void timestamp_init(TimestampConfig *tc)
{
    memset(tc, 0, sizeof(*tc));
    tc->format = 0;
    tc->position = 0;
    tc->color = 0x666666;
    tc->on_newline_only = 1;
    tc->show_elapsed = 0;
}

int timestamp_format(TimestampConfig *tc, char *buf, int buflen)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    switch (tc->format) {
        case 0: return snprintf(buf, buflen, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
        case 2: return snprintf(buf, buflen, "%04d-%02d-%02dT%02d:%02d:%02d",
                    t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        default: return snprintf(buf, buflen, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    }
}

int timestamp_elapsed(TimestampConfig *tc, char *buf, int buflen)
{
    if (!tc->last_line_time) return snprintf(buf, buflen, "+0s");
    unsigned long now = (unsigned long)time(NULL);
    unsigned long diff = now - tc->last_line_time;
    if (diff < 60) return snprintf(buf, buflen, "+%lus", diff);
    if (diff < 3600) return snprintf(buf, buflen, "+%lum%02lus", diff/60, diff%60);
    return snprintf(buf, buflen, "+%luh%02lum", diff/3600, (diff%3600)/60);
}

void timestamp_mark(TimestampConfig *tc) { tc->last_line_time = (unsigned long)time(NULL); }
void timestamp_toggle(TimestampConfig *tc) { tc->enabled = !tc->enabled; }
