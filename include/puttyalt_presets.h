#ifndef PUTTYALT_PRESETS_H
#define PUTTYALT_PRESETS_H

#define PRESET_MAX 32

typedef struct {
    char name[64];
    char description[128];
    /* terminal settings */
    int  scrollback_lines;
    int  font_size;
    char font_name[64];
    int  color_scheme;
    /* connection settings */
    int  keepalive_sec;
    int  compression;
    int  x11_forwarding;
    int  agent_forwarding;
    /* behavior */
    int  close_on_exit; /* 0=never, 1=always, 2=on clean exit */
    int  warn_on_close;
    int  bell_enabled;
    int  bracketed_paste;
} ConnectionPreset;

typedef struct {
    ConnectionPreset presets[PRESET_MAX];
    int count;
    int default_preset;
} PresetStore;

void preset_store_init(PresetStore *ps);
void preset_add_defaults(PresetStore *ps);
int  preset_add(PresetStore *ps, const char *name, const char *desc);
int  preset_remove(PresetStore *ps, int index);
ConnectionPreset *preset_get(PresetStore *ps, int index);
ConnectionPreset *preset_find(PresetStore *ps, const char *name);
int  preset_apply(ConnectionPreset *preset); /* apply to current session */

#endif
