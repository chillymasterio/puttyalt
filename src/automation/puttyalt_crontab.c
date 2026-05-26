#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_CRON 32
typedef struct { char name[64]; char command[512]; int minute; int hour; int day; int enabled; time_t last_run; } CronJob;
static CronJob g_cron[MAX_CRON]; static int g_cron_count = 0;
int crontab_add(const char *name, const char *cmd, int minute, int hour, int day) {
    if (g_cron_count >= MAX_CRON) return -1;
    CronJob *j = &g_cron[g_cron_count]; memset(j, 0, sizeof(*j));
    snprintf(j->name, 64, "%s", name); snprintf(j->command, 512, "%s", cmd);
    j->minute = minute; j->hour = hour; j->day = day; j->enabled = 1;
    return g_cron_count++;
}
int crontab_should_run(int idx, time_t now) {
    if (idx < 0 || idx >= g_cron_count || !g_cron[idx].enabled) return 0;
    struct tm *t = localtime(&now);
    CronJob *j = &g_cron[idx];
    if (j->minute >= 0 && t->tm_min != j->minute) return 0;
    if (j->hour >= 0 && t->tm_hour != j->hour) return 0;
    if (j->day >= 0 && t->tm_mday != j->day) return 0;
    if (now - j->last_run < 60) return 0;
    return 1;
}
void crontab_mark_run(int idx) { if (idx >= 0 && idx < g_cron_count) g_cron[idx].last_run = time(NULL); }
int crontab_count(void) { return g_cron_count; }
