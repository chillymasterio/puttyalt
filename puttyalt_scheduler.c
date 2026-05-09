#include <string.h>
#include "puttyalt_scheduler.h"

void sched_init(Scheduler *s)
{
    memset(s, 0, sizeof(*s));
}

int sched_add(Scheduler *s, const char *label, const char *cmd,
              int session_id, SchedRepeat repeat, unsigned long first_run)
{
    if (s->count >= SCHED_MAX_JOBS) return -1;
    SchedJob *j = &s->jobs[s->count];
    memset(j, 0, sizeof(*j));
    strncpy(j->label, label, sizeof(j->label) - 1);
    strncpy(j->command, cmd, SCHED_CMD_LEN - 1);
    j->session_id = session_id;
    j->repeat = repeat;
    j->next_run = first_run;
    j->enabled = 1;

    switch (repeat) {
        case SCHED_DAILY:   j->interval_sec = 86400; break;
        case SCHED_WEEKLY:  j->interval_sec = 604800; break;
        default: j->interval_sec = 0; break;
    }
    return s->count++;
}

int sched_remove(Scheduler *s, int index)
{
    if (index < 0 || index >= s->count) return -1;
    for (int i = index; i < s->count - 1; i++)
        s->jobs[i] = s->jobs[i + 1];
    s->count--;
    return 0;
}

int sched_tick(Scheduler *s, unsigned long now)
{
    int fired = 0;
    for (int i = 0; i < s->count; i++) {
        SchedJob *j = &s->jobs[i];
        if (!j->enabled) continue;
        if (now < j->next_run) continue;

        /* Fire the job */
        j->run_count++;
        fired++;

        if (j->repeat == SCHED_ONCE) {
            j->enabled = 0;
        } else if (j->interval_sec > 0) {
            j->next_run += j->interval_sec;
        } else if (j->repeat == SCHED_INTERVAL && j->interval_sec > 0) {
            j->next_run += j->interval_sec;
        }
    }
    return fired;
}

int sched_enable(Scheduler *s, int index, int enabled)
{
    if (index < 0 || index >= s->count) return -1;
    s->jobs[index].enabled = enabled;
    return 0;
}
