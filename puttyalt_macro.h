/*
 * puttyalt_macro.h: Keyboard macro recording and playback.
 */

#ifndef PUTTYALT_MACRO_H
#define PUTTYALT_MACRO_H

#define MACRO_MAX_EVENTS   4096
#define MACRO_MAX_SLOTS    16
#define MACRO_NAME_LEN     64

typedef struct MacroEvent {
    unsigned char data[8];   /* key data */
    int len;                 /* bytes in data */
    unsigned int delay_ms;   /* delay since previous event */
} MacroEvent;

typedef struct Macro {
    char name[MACRO_NAME_LEN];
    MacroEvent events[MACRO_MAX_EVENTS];
    int num_events;
    int repeat_count;        /* 0 = once, -1 = infinite */
} Macro;

typedef struct MacroRecorder {
    Macro slots[MACRO_MAX_SLOTS];
    int num_macros;
    int recording;           /* -1 = not recording, else slot index */
    int playing;             /* -1 = not playing, else slot index */
    int play_pos;            /* current event index during playback */
    unsigned long last_event_time;
} MacroRecorder;

void macro_init(MacroRecorder *rec);
int  macro_start_recording(MacroRecorder *rec, int slot, const char *name);
int  macro_stop_recording(MacroRecorder *rec);
int  macro_record_event(MacroRecorder *rec, const unsigned char *data,
                        int len, unsigned long time_ms);
int  macro_play(MacroRecorder *rec, int slot);
int  macro_stop_playback(MacroRecorder *rec);
const MacroEvent *macro_next_event(MacroRecorder *rec);
int  macro_delete(MacroRecorder *rec, int slot);
int  macro_rename(MacroRecorder *rec, int slot, const char *new_name);

#endif /* PUTTYALT_MACRO_H */
