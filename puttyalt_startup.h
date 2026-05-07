#ifndef PUTTYALT_STARTUP_H
#define PUTTYALT_STARTUP_H

#define STARTUP_MAX_CMDS   16
#define STARTUP_CMD_LEN    512

typedef struct StartupCommand {
    char command[STARTUP_CMD_LEN];
    int delay_ms;          /* delay before executing */
    int wait_for_prompt;   /* wait for shell prompt first */
    int enabled;
} StartupCommand;

typedef struct StartupRunner {
    StartupCommand commands[STARTUP_MAX_CMDS];
    int count;
    int current;
    int running;
} StartupRunner;

void startup_init(StartupRunner *sr);
int  startup_add(StartupRunner *sr, const char *cmd, int delay_ms);
int  startup_remove(StartupRunner *sr, int index);
void startup_begin(StartupRunner *sr);
const StartupCommand *startup_next(StartupRunner *sr);
void startup_reset(StartupRunner *sr);

#endif
