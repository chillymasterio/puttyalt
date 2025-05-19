#ifndef PUTTYALT_TRIGGERSV2_H
#define PUTTYALT_TRIGGERSV2_H

#define TRIG_MAX        32
#define TRIG_PAT_LEN    256
#define TRIG_CMD_LEN    512

typedef enum {
    TRIG_ACT_NOTIFY = 0,
    TRIG_ACT_SOUND,
    TRIG_ACT_HIGHLIGHT,
    TRIG_ACT_EXEC,
    TRIG_ACT_LOG,
    TRIG_ACT_SEND_KEYS
} TriggerAction;

typedef struct TriggerRule {
    char pattern[TRIG_PAT_LEN];
    TriggerAction action;
    char action_data[TRIG_CMD_LEN];  /* cmd to exec, sound path, etc */
    int enabled;
    int case_sensitive;
    int match_count;
    unsigned long last_match;
} TriggerRule;

typedef struct TriggerEngine {
    TriggerRule rules[TRIG_MAX];
    int count;
    int global_enabled;
} TriggerEngine;

void triggers_init(TriggerEngine *te);
int  triggers_add(TriggerEngine *te, const char *pattern,
                  TriggerAction action, const char *action_data);
int  triggers_remove(TriggerEngine *te, int index);
int  triggers_check(TriggerEngine *te, const char *line, unsigned long now);
int  triggers_enable(TriggerEngine *te, int index, int enabled);

#endif
