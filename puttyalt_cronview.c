#include "puttyalt_cronview.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void cronview_init(CronViewer *cv) { memset(cv, 0, sizeof(*cv)); }

int cronview_parse(CronViewer *cv, const char *text)
{
    if (!text) return -1;
    snprintf(cv->raw_crontab, sizeof(cv->raw_crontab), "%s", text);
    cv->raw_len = (int)strlen(cv->raw_crontab);
    cv->count = 0;

    const char *p = text;
    char line[1024];
    while (*p && cv->count < CRON_MAX_ENTRIES) {
        const char *eol = strchr(p, '\n');
        int len = eol ? (int)(eol - p) : (int)strlen(p);
        if (len >= (int)sizeof(line)) len = sizeof(line) - 1;
        memcpy(line, p, len); line[len] = '\0';
        p = eol ? eol + 1 : p + len;

        /* skip comments and empty lines */
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (!*trimmed || *trimmed == '#') continue;
        /* skip variable assignments (contain = before first space-group) */
        char *eq = strchr(trimmed, '=');
        char *sp = strchr(trimmed, ' ');
        if (eq && (!sp || eq < sp)) continue;

        /* parse: min hour dom month dow command */
        CronEntry *e = &cv->entries[cv->count];
        memset(e, 0, sizeof(*e));
        e->enabled = 1;

        /* extract 5 schedule fields */
        char *tok = trimmed;
        int fields = 0;
        int sched_end = 0;
        for (int f = 0; f < 5 && *tok; f++) {
            while (*tok == ' ' || *tok == '\t') tok++;
            char *start = tok;
            while (*tok && *tok != ' ' && *tok != '\t') tok++;
            fields++;
            sched_end = (int)(tok - trimmed);
        }
        if (fields < 5) continue;
        if (sched_end >= (int)sizeof(e->schedule)) sched_end = sizeof(e->schedule) - 1;
        memcpy(e->schedule, trimmed, sched_end);
        e->schedule[sched_end] = '\0';

        while (*tok == ' ' || *tok == '\t') tok++;
        snprintf(e->command, sizeof(e->command), "%s", tok);
        cv->count++;
    }
    return cv->count;
}

int cronview_toggle(CronViewer *cv, int index)
{
    if (index < 0 || index >= cv->count) return -1;
    cv->entries[index].enabled = !cv->entries[index].enabled;
    return 0;
}

CronEntry *cronview_get(CronViewer *cv, int index)
{
    return (index >= 0 && index < cv->count) ? &cv->entries[index] : NULL;
}

const char *cronview_describe_schedule(const char *schedule, char *buf, int buflen)
{
    if (!schedule || !buf) return "";
    /* common patterns */
    if (strcmp(schedule, "* * * * *") == 0) { snprintf(buf, buflen, "Every minute"); return buf; }
    if (strcmp(schedule, "0 * * * *") == 0) { snprintf(buf, buflen, "Every hour"); return buf; }
    if (strcmp(schedule, "0 0 * * *") == 0) { snprintf(buf, buflen, "Daily at midnight"); return buf; }
    if (strcmp(schedule, "0 0 * * 0") == 0) { snprintf(buf, buflen, "Weekly on Sunday"); return buf; }
    if (strcmp(schedule, "0 0 1 * *") == 0) { snprintf(buf, buflen, "Monthly on the 1st"); return buf; }
    if (strncmp(schedule, "*/5 ", 4) == 0) { snprintf(buf, buflen, "Every 5 minutes"); return buf; }
    if (strncmp(schedule, "*/15 ", 5) == 0) { snprintf(buf, buflen, "Every 15 minutes"); return buf; }
    if (strncmp(schedule, "*/30 ", 5) == 0) { snprintf(buf, buflen, "Every 30 minutes"); return buf; }
    snprintf(buf, buflen, "%s", schedule);
    return buf;
}
