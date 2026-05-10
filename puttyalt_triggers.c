/*
 * puttyalt_triggers.c: Terminal output trigger engine.
 */

#include "puttyalt_triggers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void triggers_init(TriggerEngine *eng)
{
    memset(eng, 0, sizeof(*eng));
}

int triggers_add(TriggerEngine *eng, const Trigger *t)
{
    if (eng->count >= MAX_TRIGGERS)
        return -1;
    memcpy(&eng->triggers[eng->count], t, sizeof(Trigger));
    return eng->count++;
}

int triggers_remove(TriggerEngine *eng, int index)
{
    if (index < 0 || index >= eng->count)
        return -1;
    for (int i = index; i < eng->count - 1; i++)
        eng->triggers[i] = eng->triggers[i + 1];
    eng->count--;
    return 0;
}

/*
 * Case-insensitive substring search.
 */
static const char *strcasestr_local(const char *haystack, const char *needle)
{
    size_t nlen = strlen(needle);
    if (nlen == 0) return haystack;

    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            size_t i;
            for (i = 1; i < nlen; i++) {
                if (tolower((unsigned char)haystack[i]) !=
                    tolower((unsigned char)needle[i]))
                    break;
            }
            if (i == nlen)
                return haystack;
        }
    }
    return NULL;
}

static int pattern_matches(const Trigger *t, const char *line)
{
    if (t->case_sensitive)
        return strstr(line, t->pattern) != NULL;
    else
        return strcasestr_local(line, t->pattern) != NULL;
}

int triggers_process_line(TriggerEngine *eng, const char *line, long now_ms)
{
    int fired = 0;

    for (int i = 0; i < eng->count; i++) {
        Trigger *t = &eng->triggers[i];

        if (!t->enabled)
            continue;

        /* Cooldown check */
        if (t->cooldown_ms > 0 && t->last_fired > 0) {
            if (now_ms - t->last_fired < t->cooldown_ms)
                continue;
        }

        if (pattern_matches(t, line)) {
            t->last_fired = now_ms;
            fired++;

            /* Action dispatch is handled by the GUI layer,
             * but we log here for diagnostic purposes */
        }
    }

    return fired;
}

int triggers_load(TriggerEngine *eng, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[8192];
    Trigger *cur = NULL;

    if (!f) return -1;

    triggers_init(eng);

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (strcmp(line, "[trigger]") == 0) {
            if (eng->count >= MAX_TRIGGERS) break;
            cur = &eng->triggers[eng->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            cur->cooldown_ms = 5000;
            continue;
        }
        if (!cur) continue;

        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, sizeof(cur->name), "%s", line + 5);
        else if (strncmp(line, "pattern=", 8) == 0)
            snprintf(cur->pattern, sizeof(cur->pattern), "%s", line + 8);
        else if (strncmp(line, "action=", 7) == 0)
            cur->action = atoi(line + 7);
        else if (strncmp(line, "action_param=", 13) == 0)
            snprintf(cur->action_param, sizeof(cur->action_param), "%s", line + 13);
        else if (strncmp(line, "case_sensitive=", 15) == 0)
            cur->case_sensitive = atoi(line + 15);
        else if (strncmp(line, "cooldown=", 9) == 0)
            cur->cooldown_ms = atoi(line + 9);
        else if (strncmp(line, "enabled=", 8) == 0)
            cur->enabled = atoi(line + 8);
    }

    fclose(f);
    return 0;
}

int triggers_save(const TriggerEngine *eng, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    for (int i = 0; i < eng->count; i++) {
        const Trigger *t = &eng->triggers[i];
        fprintf(f, "[trigger]\n");
        fprintf(f, "name=%s\n", t->name);
        fprintf(f, "pattern=%s\n", t->pattern);
        fprintf(f, "action=%d\n", t->action);
        fprintf(f, "action_param=%s\n", t->action_param);
        fprintf(f, "case_sensitive=%d\n", t->case_sensitive);
        fprintf(f, "cooldown=%d\n", t->cooldown_ms);
        fprintf(f, "enabled=%d\n", t->enabled);
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}
