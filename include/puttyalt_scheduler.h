#ifndef PUTTYALT_SCHEDULER_H
#define PUTTYALT_SCHEDULER_H

#define SCHED_MAX_TASKS 64

typedef enum {
    SCHED_ONCE, SCHED_INTERVAL, SCHED_CRON, SCHED_ON_CONNECT
} SchedType;

typedef struct {
    char name[64];
    char command[512];
    SchedType type;
    int interval_sec;
    long next_run;
    long last_run;
    int session_idx;
    int enabled;
    int run_count;
    int max_runs;  /* 0=unlimited */
} SchedTask;

typedef struct {
    SchedTask tasks[SCHED_MAX_TASKS];
    int count;
    int running;
} Scheduler;

int  sched_init(Scheduler *s);
int  sched_add(Scheduler *s, const char *name, const char *cmd,
               SchedType type, int interval);
int  sched_remove(Scheduler *s, int idx);
int  sched_tick(Scheduler *s, long now);
int  sched_enable(Scheduler *s, int idx, int enabled);
void sched_destroy(Scheduler *s);

#endif
