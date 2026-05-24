#ifndef PUTTYALT_MACRO_H
#define PUTTYALT_MACRO_H

#define MACRO_MAX_NAME    64
#define MACRO_MAX_STEPS   256
#define MACRO_MAX_MACROS  64
#define MACRO_MAX_DATA    512

typedef enum {
    MACRO_SEND_TEXT = 0,
    MACRO_SEND_KEY,
    MACRO_WAIT_MS,
    MACRO_WAIT_PATTERN,
    MACRO_SEND_FILE,
    MACRO_SCREENSHOT
} MacroStepType;

typedef struct {
    MacroStepType type;
    char          data[MACRO_MAX_DATA];
    int           delay_ms;
} MacroStep;

typedef struct {
    char      name[MACRO_MAX_NAME];
    MacroStep steps[MACRO_MAX_STEPS];
    int       step_count;
    int       loop_count;      /* 0 = no loop, -1 = infinite */
    int       hotkey;
    int       hotkey_mod;
} Macro;

typedef struct {
    Macro  macros[MACRO_MAX_MACROS];
    int    count;
    int    recording;
    int    playing;
    int    current_macro;
    int    current_step;
    int    play_loop;
} MacroEngine;

void macro_init(MacroEngine *me);
int  macro_create(MacroEngine *me, const char *name);
int  macro_delete(MacroEngine *me, int index);
void macro_start_record(MacroEngine *me, int index);
void macro_stop_record(MacroEngine *me);
int  macro_add_step(MacroEngine *me, MacroStepType type,
                    const char *data, int delay_ms);
int  macro_play(MacroEngine *me, int index);
void macro_stop_play(MacroEngine *me);
int  macro_step(MacroEngine *me, char *out_data, int outsz,
                MacroStepType *out_type);
int  macro_find(const MacroEngine *me, const char *name);
int  macro_load(MacroEngine *me, const char *path);
int  macro_save(const MacroEngine *me, const char *path);
int  macro_set_hotkey(MacroEngine *me, int index, int key, int mod);

#endif
