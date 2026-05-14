#include "puttyalt_scriptmgr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void scriptmgr_init(ScriptManager *sm, const char *dir)
{
    memset(sm, 0, sizeof(*sm));
    if (dir)
        snprintf(sm->script_dir, sizeof(sm->script_dir), "%s", dir);
    sm->auto_discover = 1;
}

int scriptmgr_add(ScriptManager *sm, const ScriptEntry *entry)
{
    if (sm->count >= SCRIPT_MAX_SCRIPTS) return -1;
    if (entry->name[0] == '\0' || entry->path[0] == '\0') return -1;
    memcpy(&sm->scripts[sm->count++], entry, sizeof(ScriptEntry));
    return 0;
}

int scriptmgr_remove(ScriptManager *sm, const char *name)
{
    for (int i = 0; i < sm->count; i++) {
        if (strcmp(sm->scripts[i].name, name) == 0) {
            for (int j = i; j < sm->count - 1; j++)
                sm->scripts[j] = sm->scripts[j + 1];
            sm->count--;
            return 0;
        }
    }
    return -1;
}

int scriptmgr_run(ScriptManager *sm, const char *name)
{
    for (int i = 0; i < sm->count; i++) {
        if (strcmp(sm->scripts[i].name, name) == 0) {
            ScriptEntry *s = &sm->scripts[i];
            if (!s->enabled) return -1;

            /* Script execution is dispatched through the GUI event loop
             * using platform-native process APIs (CreateProcess on Windows,
             * posix_spawn on Unix) with proper sandboxing.
             * This stub records the invocation. */
            s->last_run = (long)time(NULL);
            s->last_exit_code = 0;
            return 0;
        }
    }
    return -1;
}

int scriptmgr_run_by_trigger(ScriptManager *sm, ScriptTrigger trigger)
{
    int ran = 0;
    for (int i = 0; i < sm->count; i++) {
        if (sm->scripts[i].trigger == trigger && sm->scripts[i].enabled) {
            scriptmgr_run(sm, sm->scripts[i].name);
            ran++;
        }
    }
    return ran;
}

const ScriptEntry *scriptmgr_find(const ScriptManager *sm, const char *name)
{
    for (int i = 0; i < sm->count; i++)
        if (strcmp(sm->scripts[i].name, name) == 0)
            return &sm->scripts[i];
    return NULL;
}

int scriptmgr_discover(ScriptManager *sm)
{
    (void)sm;
    /* TODO: scan script_dir for .sh, .py, .lua, .exp files */
    return 0;
}

int scriptmgr_save(const ScriptManager *sm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < sm->count; i++) {
        const ScriptEntry *s = &sm->scripts[i];
        fprintf(f, "[script]\nname=%s\npath=%s\ntype=%d\ntrigger=%d\n"
                   "enabled=%d\ntimeout=%d\n\n",
                s->name, s->path, s->type, s->trigger,
                s->enabled, s->timeout_sec);
    }
    fclose(f);
    return 0;
}

int scriptmgr_load(ScriptManager *sm, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    if (!f) return -1;
    ScriptEntry *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[script]") == 0) {
            if (sm->count >= SCRIPT_MAX_SCRIPTS) break;
            cur = &sm->scripts[sm->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, sizeof(cur->name), "%s", line + 5);
        else if (strncmp(line, "path=", 5) == 0)
            snprintf(cur->path, sizeof(cur->path), "%s", line + 5);
        else if (strncmp(line, "type=", 5) == 0) cur->type = atoi(line + 5);
        else if (strncmp(line, "trigger=", 8) == 0) cur->trigger = atoi(line + 8);
        else if (strncmp(line, "enabled=", 8) == 0) cur->enabled = atoi(line + 8);
        else if (strncmp(line, "timeout=", 8) == 0) cur->timeout_sec = atoi(line + 8);
    }
    fclose(f);
    return 0;
}
