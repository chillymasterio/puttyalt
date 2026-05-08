#ifndef PUTTYALT_SCHEDULER_H
#define PUTTYALT_SCHEDULER_H

#define SCHED_MAX_JOBS  32
#define SCHED_CMD_LEN   512

typedef enum {
    SCHED_ONCE = 0,
    SCHED_DAILY,
    SCHED_WEEKLY,
    SCHED_INTERVAL
} SchedRepeat;

typedef struct SchedJob {
    char label[64];
    char command[SCHED_CMD_LEN];
    int session_id;
    SchedRepeat repeat;
    unsigned long next_run;
    unsigned long interval_sec;
    int enabled;
    int run_count;
} SchedJob;

typedef struct Scheduler {
    SchedJob jobs[SCHED_MAX_JOBS];
    int count;
} Scheduler;

void sched_init(Scheduler *s);
int  sched_add(Scheduler *s, const char *label, const char *cmd,
               int session_id, SchedRepeat repeat, unsigned long first_run);
int  sched_remove(Scheduler *s, int index);
int  sched_tick(Scheduler *s, unsigned long now);
int  sched_enable(Scheduler *s, int index, int enabled);

#endif
