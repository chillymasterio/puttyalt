#ifndef PUTTYALT_CRONVIEW_H
#define PUTTYALT_CRONVIEW_H

#define CRON_MAX_ENTRIES 128

typedef struct {
    char schedule[64];
    char command[512];
    char user[32];
    int  enabled;
    char next_run[32];
    char description[128];
} CronEntry;

typedef struct {
    CronEntry entries[CRON_MAX_ENTRIES];
    int count;
    char raw_crontab[16384];
    int  raw_len;
} CronViewer;

void cronview_init(CronViewer *cv);
int  cronview_parse(CronViewer *cv, const char *crontab_text);
int  cronview_toggle(CronViewer *cv, int index);
CronEntry *cronview_get(CronViewer *cv, int index);
const char *cronview_describe_schedule(const char *schedule, char *buf, int buflen);

#endif
