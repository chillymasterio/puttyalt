#ifndef PUTTYALT_PIPELINE_H
#define PUTTYALT_PIPELINE_H

#define PIPE_MAX_STEPS    32
#define PIPE_MAX_PIPES    16
#define PIPE_MAX_CMD      512

typedef enum {
    PIPE_STEP_COMMAND,
    PIPE_STEP_WAIT,
    PIPE_STEP_EXPECT,
    PIPE_STEP_TRANSFER,
    PIPE_STEP_SWITCH_SESSION,
    PIPE_STEP_CONDITION,
    PIPE_STEP_LOOP
} PipeStepType;

typedef enum {
    PIPE_STATUS_IDLE,
    PIPE_STATUS_RUNNING,
    PIPE_STATUS_PAUSED,
    PIPE_STATUS_DONE,
    PIPE_STATUS_ERROR
} PipeStatus;

typedef struct {
    PipeStepType type;
    char command[PIPE_MAX_CMD];
    int session_idx;
    int timeout_ms;
    int condition_step;  /* goto on failure */
    int repeat;
    int delay_ms;
} PipeStep;

typedef struct {
    char name[64];
    PipeStep steps[PIPE_MAX_STEPS];
    int step_count;
    int current_step;
    PipeStatus status;
    long started_at;
    long finished_at;
    int error_step;
    char error_msg[256];
} Pipeline;

typedef struct {
    Pipeline pipes[PIPE_MAX_PIPES];
    int pipe_count;
    int active_pipe;
} PipeManager;

int  pipe_init(PipeManager *pm);
void pipe_destroy(PipeManager *pm);
int  pipe_create(PipeManager *pm, const char *name);
int  pipe_add_step(PipeManager *pm, int pipe_idx, PipeStepType type,
                   const char *cmd, int session, int timeout);
int  pipe_start(PipeManager *pm, int pipe_idx);
int  pipe_stop(PipeManager *pm, int pipe_idx);
int  pipe_pause(PipeManager *pm, int pipe_idx);
int  pipe_resume(PipeManager *pm, int pipe_idx);
PipeStatus pipe_status(const PipeManager *pm, int pipe_idx);
int  pipe_load(PipeManager *pm, const char *path);
int  pipe_shutdown(PipeManager *pm);
int  pipe_save(const PipeManager *pm, const char *path);

#endif
