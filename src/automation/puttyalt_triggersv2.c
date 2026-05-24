#include <string.h>
#include <ctype.h>
#include "puttyalt_triggersv2.h"

void triggers_init(TriggerEngine *te)
{
    memset(te, 0, sizeof(*te));
    te->global_enabled = 1;
}

int triggers_add(TriggerEngine *te, const char *pattern,
                 TriggerAction action, const char *action_data)
{
    if (te->count >= TRIG_MAX) return -1;
    TriggerRule *r = &te->rules[te->count];
    memset(r, 0, sizeof(*r));
    strncpy(r->pattern, pattern, TRIG_PAT_LEN - 1);
    r->action = action;
    if (action_data)
        strncpy(r->action_data, action_data, TRIG_CMD_LEN - 1);
    r->enabled = 1;
    return te->count++;
}

int triggers_remove(TriggerEngine *te, int index)
{
    if (index < 0 || index >= te->count) return -1;
    for (int i = index; i < te->count - 1; i++)
        te->rules[i] = te->rules[i + 1];
    te->count--;
    return 0;
}

static int simple_match(const char *text, const char *pat, int cs)
{
    int tlen = strlen(text);
    int plen = strlen(pat);
    for (int i = 0; i <= tlen - plen; i++) {
        int match = 1;
        for (int j = 0; j < plen; j++) {
            char tc = text[i + j], pc = pat[j];
            if (!cs) { tc = tolower((unsigned char)tc); pc = tolower((unsigned char)pc); }
            if (tc != pc) { match = 0; break; }
        }
        if (match) return 1;
    }
    return 0;
}

int triggers_check(TriggerEngine *te, const char *line, unsigned long now)
{
    if (!te->global_enabled) return 0;
    int fired = 0;
    for (int i = 0; i < te->count; i++) {
        TriggerRule *r = &te->rules[i];
        if (!r->enabled) continue;
        if (simple_match(line, r->pattern, r->case_sensitive)) {
            r->match_count++;
            r->last_match = now;
            fired++;
        }
    }
    return fired;
}

int triggers_enable(TriggerEngine *te, int index, int enabled)
{
    if (index < 0 || index >= te->count) return -1;
    te->rules[index].enabled = enabled;
    return 0;
}
