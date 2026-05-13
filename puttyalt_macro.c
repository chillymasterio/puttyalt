#include "puttyalt_macro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void macro_init(MacroEngine *me)
{
    memset(me, 0, sizeof(*me));
    me->current_macro = -1;
}

int macro_create(MacroEngine *me, const char *name)
{
    if (me->count >= MACRO_MAX_MACROS) return -1;
    Macro *m = &me->macros[me->count];
    memset(m, 0, sizeof(*m));
    snprintf(m->name, MACRO_MAX_NAME, "%s", name);
    return me->count++;
}

int macro_delete(MacroEngine *me, int index)
{
    if (index < 0 || index >= me->count) return -1;
    for (int i = index; i < me->count - 1; i++)
        me->macros[i] = me->macros[i + 1];
    me->count--;
    return 0;
}

void macro_start_record(MacroEngine *me, int index)
{
    if (index < 0 || index >= me->count) return;
    me->recording = 1;
    me->current_macro = index;
    me->macros[index].step_count = 0;
}

void macro_stop_record(MacroEngine *me)
{
    me->recording = 0;
}

int macro_add_step(MacroEngine *me, MacroStepType type,
                   const char *data, int delay_ms)
{
    if (!me->recording || me->current_macro < 0) return -1;
    Macro *m = &me->macros[me->current_macro];
    if (m->step_count >= MACRO_MAX_STEPS) return -1;
    MacroStep *s = &m->steps[m->step_count++];
    s->type = type;
    if (data) snprintf(s->data, MACRO_MAX_DATA, "%s", data);
    s->delay_ms = delay_ms;
    return 0;
}

int macro_play(MacroEngine *me, int index)
{
    if (index < 0 || index >= me->count) return -1;
    me->playing = 1;
    me->current_macro = index;
    me->current_step = 0;
    me->play_loop = 0;
    return 0;
}

void macro_stop_play(MacroEngine *me)
{
    me->playing = 0;
    me->current_step = 0;
}

int macro_step(MacroEngine *me, char *out_data, int outsz,
               MacroStepType *out_type)
{
    if (!me->playing || me->current_macro < 0) return -1;
    Macro *m = &me->macros[me->current_macro];

    if (me->current_step >= m->step_count) {
        if (m->loop_count == -1 || me->play_loop < m->loop_count - 1) {
            me->current_step = 0;
            me->play_loop++;
        } else {
            macro_stop_play(me);
            return -1;
        }
    }

    MacroStep *s = &m->steps[me->current_step++];
    *out_type = s->type;
    if (out_data) snprintf(out_data, outsz, "%s", s->data);
    return s->delay_ms;
}

int macro_find(const MacroEngine *me, const char *name)
{
    for (int i = 0; i < me->count; i++)
        if (strcmp(me->macros[i].name, name) == 0) return i;
    return -1;
}

int macro_load(MacroEngine *me, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    Macro *cur = NULL;
    if (!f) return -1;
    me->count = 0;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[macro]") == 0) {
            int idx = macro_create(me, "");
            if (idx < 0) break;
            cur = &me->macros[idx];
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, MACRO_MAX_NAME, "%s", line + 5);
        else if (strncmp(line, "loop=", 5) == 0)
            cur->loop_count = atoi(line + 5);
        else if (strncmp(line, "step=", 5) == 0 && cur->step_count < MACRO_MAX_STEPS) {
            MacroStep *s = &cur->steps[cur->step_count++];
            char *comma = strchr(line + 5, ',');
            if (comma) {
                s->type = atoi(line + 5);
                char *comma2 = strchr(comma + 1, ',');
                if (comma2) {
                    s->delay_ms = atoi(comma + 1);
                    snprintf(s->data, MACRO_MAX_DATA, "%s", comma2 + 1);
                }
            }
        }
    }
    fclose(f);
    return 0;
}

int macro_save(const MacroEngine *me, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < me->count; i++) {
        const Macro *m = &me->macros[i];
        fprintf(f, "[macro]\nname=%s\nloop=%d\n", m->name, m->loop_count);
        for (int j = 0; j < m->step_count; j++) {
            const MacroStep *s = &m->steps[j];
            fprintf(f, "step=%d,%d,%s\n", s->type, s->delay_ms, s->data);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

int macro_set_hotkey(MacroEngine *me, int index, int key, int mod)
{
    if (index < 0 || index >= me->count) return -1;
    me->macros[index].hotkey = key;
    me->macros[index].hotkey_mod = mod;
    return 0;
}
