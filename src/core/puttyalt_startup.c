#include <string.h>
#include "puttyalt_startup.h"

void startup_init(StartupRunner *sr)
{
    memset(sr, 0, sizeof(*sr));
}

int startup_add(StartupRunner *sr, const char *cmd, int delay_ms)
{
    if (sr->count >= STARTUP_MAX_CMDS) return -1;
    StartupCommand *c = &sr->commands[sr->count];
    memset(c, 0, sizeof(*c));
    strncpy(c->command, cmd, STARTUP_CMD_LEN - 1);
    c->delay_ms = delay_ms > 0 ? delay_ms : 100;
    c->wait_for_prompt = 1;
    c->enabled = 1;
    return sr->count++;
}

int startup_remove(StartupRunner *sr, int index)
{
    if (index < 0 || index >= sr->count) return -1;
    for (int i = index; i < sr->count - 1; i++)
        sr->commands[i] = sr->commands[i + 1];
    sr->count--;
    return 0;
}

void startup_begin(StartupRunner *sr)
{
    sr->current = 0;
    sr->running = 1;
}

const StartupCommand *startup_next(StartupRunner *sr)
{
    if (!sr->running) return NULL;
    while (sr->current < sr->count) {
        StartupCommand *c = &sr->commands[sr->current];
        sr->current++;
        if (c->enabled) return c;
    }
    sr->running = 0;
    return NULL;
}

void startup_reset(StartupRunner *sr)
{
    sr->current = 0;
    sr->running = 0;
}
