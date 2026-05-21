#ifndef PUTTYALT_UPTIME_H
#define PUTTYALT_UPTIME_H

typedef struct {
    long start_time;
    long pause_time;
    int running;
    int paused;
} UptimeTimer;

void uptime_init(UptimeTimer *ut);
void uptime_start(UptimeTimer *ut);
void uptime_stop(UptimeTimer *ut);
void uptime_pause(UptimeTimer *ut);
void uptime_resume(UptimeTimer *ut);
long uptime_elapsed(const UptimeTimer *ut);
void uptime_format(const UptimeTimer *ut, char *buf, int bufsz);

#endif
