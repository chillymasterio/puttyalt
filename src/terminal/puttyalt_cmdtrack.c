/*
 * puttyalt_cmdtrack.c - Shell integration command tracking for PuttyAlt.
 *
 * Sits downstream of the semantic prompt parser (OSC 133 markers). As each
 * shell command runs, cmdtrack_begin records the command text and the current
 * working directory; cmdtrack_finish closes it out with an exit code and the
 * completion timestamp. A bounded ring of recent commands is kept so callers
 * can replay history, query the most recent failure, or render a one-line
 * summary for a status bar.
 *
 * Self-contained C99 library module. No project headers, no windows.h.
 * Timestamps arrive as now_ms parameters; no time-of-day calls are made.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CMDTRACK_RING   64    /* number of commands retained             */
#define CMDTRACK_CMDLEN 256   /* command text capacity (incl NUL)        */
#define CMDTRACK_CWDLEN 256   /* working directory capacity (incl NUL)   */

typedef struct cmdtrack_entry {
    char     cmd[CMDTRACK_CMDLEN];
    char     cwd[CMDTRACK_CWDLEN];
    uint64_t start_ms;        /* timestamp at cmdtrack_begin             */
    uint64_t end_ms;          /* timestamp at cmdtrack_finish            */
    int      exit_code;       /* shell exit status                       */
    int      running;         /* 1 while command is in flight            */
} cmdtrack_entry;

typedef struct cmdtrack {
    cmdtrack_entry ring[CMDTRACK_RING];
    char     cwd[CMDTRACK_CWDLEN]; /* current cwd applied to new commands */
    int      count;           /* live entries in ring (capped at RING)   */
    int      head;            /* index of next slot to write             */
    int      active;          /* ring index of running command, or -1    */
} cmdtrack;

int cmdtrack_init(cmdtrack *t)
{
    if (!t)
        return -1;
    memset(t, 0, sizeof(*t));
    t->active = -1;
    return 0;
}

int cmdtrack_set_cwd(cmdtrack *t, const char *cwd)
{
    if (!t || !cwd)
        return -1;
    t->cwd[0] = '\0';
    strncpy(t->cwd, cwd, CMDTRACK_CWDLEN - 1);
    t->cwd[CMDTRACK_CWDLEN - 1] = '\0';
    return 0;
}

/* Map a logical position (0 = oldest) to a physical ring index. */
static int cmdtrack_phys(const cmdtrack *t, int i)
{
    int base = t->head - t->count;
    int idx = base + i;
    idx %= CMDTRACK_RING;
    if (idx < 0)
        idx += CMDTRACK_RING;
    return idx;
}

int cmdtrack_begin(cmdtrack *t, const char *cmd, uint64_t now_ms)
{
    cmdtrack_entry *e;
    if (!t || !cmd)
        return -1;
    e = &t->ring[t->head];
    memset(e, 0, sizeof(*e));
    strncpy(e->cmd, cmd, CMDTRACK_CMDLEN - 1);
    e->cmd[CMDTRACK_CMDLEN - 1] = '\0';
    memcpy(e->cwd, t->cwd, CMDTRACK_CWDLEN);
    e->start_ms = now_ms;
    e->end_ms = now_ms;
    e->exit_code = -1;
    e->running = 1;
    t->active = t->head;
    t->head = (t->head + 1) % CMDTRACK_RING;
    if (t->count < CMDTRACK_RING)
        t->count++;
    return 0;
}

int cmdtrack_finish(cmdtrack *t, int exit_code, uint64_t now_ms)
{
    cmdtrack_entry *e;
    if (!t || t->active < 0)
        return -1;
    e = &t->ring[t->active];
    if (!e->running)
        return -1;
    e->exit_code = exit_code;
    e->end_ms = (now_ms >= e->start_ms) ? now_ms : e->start_ms;
    e->running = 0;
    t->active = -1;
    return 0;
}

int cmdtrack_count(const cmdtrack *t)
{
    return t ? t->count : 0;
}

/* Fetch logical entry i (0 = oldest retained). Returns NULL if out of range. */
const cmdtrack_entry *cmdtrack_get(const cmdtrack *t, int i)
{
    if (!t || i < 0 || i >= t->count)
        return NULL;
    return &t->ring[cmdtrack_phys(t, i)];
}

/* Elapsed milliseconds for a finished entry, or -1 if running/invalid. */
int64_t cmdtrack_duration(const cmdtrack_entry *e)
{
    if (!e || e->running || e->end_ms < e->start_ms)
        return -1;
    return (int64_t)(e->end_ms - e->start_ms);
}

/* Most recent completed command with a nonzero exit, or NULL if none. */
const cmdtrack_entry *cmdtrack_last_failure(const cmdtrack *t)
{
    int i;
    if (!t)
        return NULL;
    for (i = t->count - 1; i >= 0; i--) {
        const cmdtrack_entry *e = &t->ring[cmdtrack_phys(t, i)];
        if (!e->running && e->exit_code != 0)
            return e;
    }
    return NULL;
}

/* Render a one-line summary of an entry into buf. Returns chars written or -1. */
int cmdtrack_format(const cmdtrack_entry *e, char *buf, int bufsize)
{
    int w;
    int64_t dur;
    if (!e || !buf || bufsize <= 0)
        return -1;
    dur = cmdtrack_duration(e);
    if (e->running)
        w = snprintf(buf, (size_t)bufsize, "[run] %s (%s)", e->cmd, e->cwd);
    else
        w = snprintf(buf, (size_t)bufsize, "[%d] %s (%s) %lldms",
                     e->exit_code, e->cmd, e->cwd, (long long)dur);
    if (w < 0 || w >= bufsize)
        return -1;
    return w;
}
