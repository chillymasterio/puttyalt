#include "puttyalt_presets.h"
#include <string.h>
#include <stdio.h>

void preset_store_init(PresetStore *ps)
{
    memset(ps, 0, sizeof(*ps));
    ps->default_preset = 0;
}

static void fill_preset(ConnectionPreset *p, const char *name, const char *desc,
                         int scroll, int fsize, const char *font, int keepalive,
                         int close_mode, int warn)
{
    snprintf(p->name, sizeof(p->name), "%s", name);
    snprintf(p->description, sizeof(p->description), "%s", desc);
    p->scrollback_lines = scroll;
    p->font_size = fsize;
    snprintf(p->font_name, sizeof(p->font_name), "%s", font);
    p->keepalive_sec = keepalive;
    p->close_on_exit = close_mode;
    p->warn_on_close = warn;
    p->bell_enabled = 1;
    p->bracketed_paste = 1;
}

void preset_add_defaults(PresetStore *ps)
{
    if (ps->count + 4 > PRESET_MAX) return;
    
    fill_preset(&ps->presets[ps->count++],
        "Default", "Balanced settings for general use",
        10000, 11, "Consolas", 60, 2, 1);

    fill_preset(&ps->presets[ps->count++],
        "Development", "For coding — large scrollback, small font",
        50000, 10, "Cascadia Code", 30, 0, 0);
    ps->presets[ps->count - 1].x11_forwarding = 1;
    ps->presets[ps->count - 1].agent_forwarding = 1;

    fill_preset(&ps->presets[ps->count++],
        "Production", "Safety-first for prod servers",
        5000, 12, "Consolas", 120, 0, 1);
    ps->presets[ps->count - 1].warn_on_close = 1;

    fill_preset(&ps->presets[ps->count++],
        "Presentation", "Large font, clean display for demos",
        2000, 16, "Cascadia Code", 60, 1, 0);
}

int preset_add(PresetStore *ps, const char *name, const char *desc)
{
    if (ps->count >= PRESET_MAX) return -1;
    ConnectionPreset *p = &ps->presets[ps->count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, sizeof(p->name), "%s", name);
    snprintf(p->description, sizeof(p->description), "%s", desc ? desc : "");
    p->scrollback_lines = 10000;
    p->font_size = 11;
    snprintf(p->font_name, sizeof(p->font_name), "Consolas");
    p->keepalive_sec = 60;
    p->bell_enabled = 1;
    p->bracketed_paste = 1;
    ps->count++;
    return ps->count - 1;
}

int preset_remove(PresetStore *ps, int index)
{
    if (index < 0 || index >= ps->count) return -1;
    memmove(&ps->presets[index], &ps->presets[index+1], (ps->count - index - 1) * sizeof(ConnectionPreset));
    ps->count--;
    return 0;
}

ConnectionPreset *preset_get(PresetStore *ps, int index)
{
    return (index >= 0 && index < ps->count) ? &ps->presets[index] : NULL;
}

ConnectionPreset *preset_find(PresetStore *ps, const char *name)
{
    for (int i = 0; i < ps->count; i++)
        if (strcmp(ps->presets[i].name, name) == 0) return &ps->presets[i];
    return NULL;
}

int preset_apply(ConnectionPreset *preset)
{
    (void)preset;
    /* platform-specific: would apply settings to current session */
    return 0;
}
