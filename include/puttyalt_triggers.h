/*
 * puttyalt_triggers.h: Pattern-matching triggers for terminal output.
 *
 * Define patterns that, when matched in terminal output, trigger
 * actions: visual alerts, sounds, log highlights, or command execution.
 * Useful for monitoring: "ERROR" in logs, "disk full" alerts, etc.
 */

#ifndef PUTTYALT_TRIGGERS_H
#define PUTTYALT_TRIGGERS_H

#define MAX_TRIGGERS      64
#define MAX_PATTERN_LEN   256
#define MAX_ACTION_LEN    1024

typedef enum {
    TRIGGER_ACTION_FLASH,       /* Flash the window/taskbar */
    TRIGGER_ACTION_BEEP,        /* System beep */
    TRIGGER_ACTION_HIGHLIGHT,   /* Colour the matching line */
    TRIGGER_ACTION_LOG,         /* Write to a trigger log file */
    TRIGGER_ACTION_EXEC,        /* Run an external command */
    TRIGGER_ACTION_NOTIFY       /* Desktop notification */
} TriggerAction;

typedef struct Trigger {
    char name[64];
    char pattern[MAX_PATTERN_LEN];     /* simple substring or glob */
    int use_regex;                      /* future: regex support */
    int case_sensitive;
    int enabled;
    TriggerAction action;
    char action_param[MAX_ACTION_LEN]; /* action-specific: command, log path, etc. */
    int cooldown_ms;                   /* minimum interval between fires */
    long last_fired;                   /* timestamp of last trigger */
} Trigger;

typedef struct TriggerEngine {
    Trigger triggers[MAX_TRIGGERS];
    int count;
} TriggerEngine;

/* Initialise the trigger engine */
void triggers_init(TriggerEngine *eng);

/* Add a trigger.  Returns index or -1 on error. */
int triggers_add(TriggerEngine *eng, const Trigger *t);

/* Remove a trigger by index */
int triggers_remove(TriggerEngine *eng, int index);

/* Process a line of terminal output against all triggers.
 * Returns the number of triggers that fired. */
int triggers_process_line(TriggerEngine *eng, const char *line, long now_ms);

/* Load/save triggers from/to config file */
int triggers_load(TriggerEngine *eng, const char *path);
int triggers_save(const TriggerEngine *eng, const char *path);

#endif /* PUTTYALT_TRIGGERS_H */
