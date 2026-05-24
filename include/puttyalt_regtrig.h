#ifndef PUTTYALT_REGTRIG_H
#define PUTTYALT_REGTRIG_H

#define TRIG_MAX 32
#define TRIG_ACT_NOTIFY  0
#define TRIG_ACT_LOG     1
#define TRIG_ACT_SOUND   2
#define TRIG_ACT_COMMAND 3
#define TRIG_ACT_HIGHLIGHT 4

typedef struct {
    char name[64];
    char pattern[256];
    int  action;
    char action_data[512]; /* command to run, sound file, highlight color */
    int  enabled;
    int  case_sensitive;
    int  match_count;
    unsigned long last_match;
    int  cooldown_sec; /* min seconds between triggers */
} RegexTrigger;

typedef struct {
    RegexTrigger triggers[TRIG_MAX];
    int count;
    int global_enabled;
} TriggerEngine;

void trigeng_init(TriggerEngine *te);
int  trigeng_add(TriggerEngine *te, const char *name, const char *pattern, int action);
int  trigeng_remove(TriggerEngine *te, int index);
int  trigeng_check(TriggerEngine *te, const char *line, int *matched_indices, int max);
void trigeng_add_defaults(TriggerEngine *te);
void trigeng_toggle(TriggerEngine *te, int index);
RegexTrigger *trigeng_get(TriggerEngine *te, int index);

#endif
