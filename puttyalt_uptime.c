#include "puttyalt_uptime.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void uptime_init(UptimeTimer *ut)
{
    memset(ut, 0, sizeof(*ut));
}

void uptime_start(UptimeTimer *ut)
{
    ut->start_time = (long)time(NULL);
    ut->running = 1;
    ut->paused = 0;
}

void uptime_stop(UptimeTimer *ut)
{
    ut->running = 0;
    ut->paused = 0;
}

void uptime_pause(UptimeTimer *ut)
{
    if (ut->running && !ut->paused) {
        ut->pause_time = (long)time(NULL);
        ut->paused = 1;
    }
}

void uptime_resume(UptimeTimer *ut)
{
    if (ut->paused) {
        long paused_duration = (long)time(NULL) - ut->pause_time;
        ut->start_time += paused_duration;
        ut->paused = 0;
    }
}

long uptime_elapsed(const UptimeTimer *ut)
{
    if (!ut->running) return 0;
    if (ut->paused) return ut->pause_time - ut->start_time;
    return (long)time(NULL) - ut->start_time;
}

void uptime_format(const UptimeTimer *ut, char *buf, int bufsz)
{
    long secs = uptime_elapsed(ut);
    if (secs < 0) secs = 0;

    int hours = (int)(secs / 3600);
    int mins = (int)((secs % 3600) / 60);
    int s = (int)(secs % 60);

    if (hours > 0)
        snprintf(buf, bufsz, "%dh %02dm %02ds", hours, mins, s);
    else if (mins > 0)
        snprintf(buf, bufsz, "%dm %02ds", mins, s);
    else
        snprintf(buf, bufsz, "%ds", s);
}
