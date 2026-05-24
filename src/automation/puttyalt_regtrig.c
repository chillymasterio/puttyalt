#include <stdio.h>
#include "puttyalt_regtrig.h"
#include <string.h>
#include <time.h>
#include <ctype.h>

void trigeng_init(TriggerEngine *te) { memset(te, 0, sizeof(*te)); te->global_enabled = 1; }

int trigeng_add(TriggerEngine *te, const char *name, const char *pattern, int action)
{
    if (te->count >= TRIG_MAX) return -1;
    RegexTrigger *t = &te->triggers[te->count++];
    memset(t, 0, sizeof(*t));
    snprintf(t->name, sizeof(t->name), "%s", name);
    snprintf(t->pattern, sizeof(t->pattern), "%s", pattern);
    t->action = action;
    t->enabled = 1;
    t->cooldown_sec = 5;
    return te->count - 1;
}

int trigeng_remove(TriggerEngine *te, int index)
{
    if (index < 0 || index >= te->count) return -1;
    memmove(&te->triggers[index], &te->triggers[index+1], (te->count-index-1)*sizeof(RegexTrigger));
    te->count--;
    return 0;
}

/* simple substring pattern match (no full regex for portability) */
static int pattern_match(const char *text, const char *pat, int case_sensitive)
{
    int tlen = (int)strlen(text), plen = (int)strlen(pat);
    for (int i = 0; i <= tlen - plen; i++) {
        int ok = 1;
        for (int j = 0; j < plen; j++) {
            char a = case_sensitive ? text[i+j] : tolower((unsigned char)text[i+j]);
            char b = case_sensitive ? pat[j] : tolower((unsigned char)pat[j]);
            if (a != b) { ok = 0; break; }
        }
        if (ok) return 1;
    }
    return 0;
}

int trigeng_check(TriggerEngine *te, const char *line, int *matched, int max)
{
    if (!te->global_enabled || !line) return 0;
    int n = 0;
    unsigned long now = (unsigned long)time(NULL);
    for (int i = 0; i < te->count && n < max; i++) {
        RegexTrigger *t = &te->triggers[i];
        if (!t->enabled) continue;
        if (t->cooldown_sec > 0 && t->last_match > 0 && (now - t->last_match) < (unsigned long)t->cooldown_sec)
            continue;
        if (pattern_match(line, t->pattern, t->case_sensitive)) {
            t->match_count++;
            t->last_match = now;
            matched[n++] = i;
        }
    }
    return n;
}

void trigeng_add_defaults(TriggerEngine *te)
{
    trigeng_add(te, "Error detected", "error", TRIG_ACT_NOTIFY);
    trigeng_add(te, "Permission denied", "Permission denied", TRIG_ACT_SOUND);
    trigeng_add(te, "Connection refused", "Connection refused", TRIG_ACT_NOTIFY);
    trigeng_add(te, "OOM Killer", "Out of memory", TRIG_ACT_NOTIFY);
    trigeng_add(te, "Disk full", "No space left", TRIG_ACT_NOTIFY);
}

void trigeng_toggle(TriggerEngine *te, int index)
{
    if (index >= 0 && index < te->count) te->triggers[index].enabled = !te->triggers[index].enabled;
}

RegexTrigger *trigeng_get(TriggerEngine *te, int index)
{
    return (index >= 0 && index < te->count) ? &te->triggers[index] : NULL;
}
