#include "puttyalt_pipeline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pipe_init(PipeManager *pm)
{
    memset(pm, 0, sizeof(*pm));
    pm->active_pipe = -1;
    return 0;
}

void pipe_destroy(PipeManager *pm)
{
    memset(pm, 0, sizeof(*pm));
}

int pipe_create(PipeManager *pm, const char *name)
{
    if (pm->pipe_count >= PIPE_MAX_PIPES) return -1;
    Pipeline *p = &pm->pipes[pm->pipe_count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, sizeof(p->name), "%s", name ? name : "Pipeline");
    p->status = PIPE_STATUS_IDLE;
    pm->pipe_count++;
    return pm->pipe_count - 1;
}

int pipe_add_step(PipeManager *pm, int pipe_idx, PipeStepType type,
                  const char *cmd, int session, int timeout)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return -1;
    Pipeline *p = &pm->pipes[pipe_idx];
    if (p->step_count >= PIPE_MAX_STEPS) return -1;

    PipeStep *s = &p->steps[p->step_count];
    memset(s, 0, sizeof(*s));
    s->type = type;
    if (cmd) snprintf(s->command, sizeof(s->command), "%s", cmd);
    s->session_idx = session;
    s->timeout_ms = timeout > 0 ? timeout : 10000;
    s->condition_step = -1;

    p->step_count++;
    return p->step_count - 1;
}

int pipe_start(PipeManager *pm, int pipe_idx)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return -1;
    Pipeline *p = &pm->pipes[pipe_idx];
    if (p->step_count == 0) return -1;

    p->status = PIPE_STATUS_RUNNING;
    p->current_step = 0;
    p->started_at = (long)time(NULL);
    p->error_step = -1;
    p->error_msg[0] = '\0';
    pm->active_pipe = pipe_idx;
    return 0;
}

int pipe_stop(PipeManager *pm, int pipe_idx)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return -1;
    pm->pipes[pipe_idx].status = PIPE_STATUS_IDLE;
    pm->pipes[pipe_idx].finished_at = (long)time(NULL);
    if (pm->active_pipe == pipe_idx)
        pm->active_pipe = -1;
    return 0;
}

int pipe_pause(PipeManager *pm, int pipe_idx)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return -1;
    if (pm->pipes[pipe_idx].status == PIPE_STATUS_RUNNING)
        pm->pipes[pipe_idx].status = PIPE_STATUS_PAUSED;
    return 0;
}

int pipe_resume(PipeManager *pm, int pipe_idx)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return -1;
    if (pm->pipes[pipe_idx].status == PIPE_STATUS_PAUSED)
        pm->pipes[pipe_idx].status = PIPE_STATUS_RUNNING;
    return 0;
}

PipeStatus pipe_status(const PipeManager *pm, int pipe_idx)
{
    if (pipe_idx < 0 || pipe_idx >= pm->pipe_count) return PIPE_STATUS_ERROR;
    return pm->pipes[pipe_idx].status;
}

int pipe_load(PipeManager *pm, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[1024];
    int cur_pipe = -1;

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!line[0] || line[0] == '#') continue;

        if (strncmp(line, "pipeline:", 9) == 0) {
            cur_pipe = pipe_create(pm, line + 9);
        } else if (cur_pipe >= 0 && strncmp(line, "run:", 4) == 0) {
            pipe_add_step(pm, cur_pipe, PIPE_STEP_COMMAND, line + 4, 0, 0);
        } else if (cur_pipe >= 0 && strncmp(line, "wait:", 5) == 0) {
            pipe_add_step(pm, cur_pipe, PIPE_STEP_WAIT, NULL, 0, atoi(line + 5));
        } else if (cur_pipe >= 0 && strncmp(line, "expect:", 7) == 0) {
            pipe_add_step(pm, cur_pipe, PIPE_STEP_EXPECT, line + 7, 0, 0);
        }
    }
    fclose(f);
    return 0;
}

int pipe_save(const PipeManager *pm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < pm->pipe_count; i++) {
        const Pipeline *p = &pm->pipes[i];
        fprintf(f, "pipeline:%s\n", p->name);
        for (int j = 0; j < p->step_count; j++) {
            const PipeStep *s = &p->steps[j];
            switch (s->type) {
            case PIPE_STEP_COMMAND:
                fprintf(f, "run:%s\n", s->command); break;
            case PIPE_STEP_WAIT:
                fprintf(f, "wait:%d\n", s->timeout_ms); break;
            case PIPE_STEP_EXPECT:
                fprintf(f, "expect:%s\n", s->command); break;
            default: break;
            }
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
