#include "puttyalt_scheduler.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int sched_init(Scheduler *s)
{
    memset(s, 0, sizeof(*s));
    s->running = 1;
    return 0;
}

int sched_add(Scheduler *s, const char *name, const char *cmd,
              SchedType type, int interval)
{
    if (s->count >= SCHED_MAX_TASKS) return -1;
    SchedTask *t = &s->tasks[s->count];
    memset(t, 0, sizeof(*t));
    snprintf(t->name, sizeof(t->name), "%s", name);
    snprintf(t->command, sizeof(t->command), "%s", cmd);
    t->type = type;
    t->interval_sec = interval > 0 ? interval : 60;
    t->next_run = (long)time(NULL) + t->interval_sec;
    t->enabled = 1;
    s->count++;
    return s->count - 1;
}

int sched_remove(Scheduler *s, int idx)
{
    if (idx < 0 || idx >= s->count) return -1;
    for (int i = idx; i < s->count - 1; i++)
        s->tasks[i] = s->tasks[i + 1];
    s->count--;
    return 0;
}

int sched_tick(Scheduler *s, long now)
{
    if (!s->running) return 0;
    int executed = 0;
    for (int i = 0; i < s->count; i++) {
        SchedTask *t = &s->tasks[i];
        if (!t->enabled) continue;
        if (t->max_runs > 0 && t->run_count >= t->max_runs) continue;
        if (now >= t->next_run) {
            t->last_run = now;
            t->run_count++;
            executed++;
            if (t->type == SCHED_INTERVAL)
                t->next_run = now + t->interval_sec;
            else if (t->type == SCHED_ONCE)
                t->enabled = 0;
        }
    }
    return executed;
}

int sched_enable(Scheduler *s, int idx, int enabled)
{
    if (idx < 0 || idx >= s->count) return -1;
    s->tasks[idx].enabled = enabled;
    return 0;
}

void sched_destroy(Scheduler *s) { memset(s, 0, sizeof(*s)); }
