#ifndef PUTTYALT_INPUTRECORD_H
#define PUTTYALT_INPUTRECORD_H

#define IREC_MAX_EVENTS 4096

typedef struct {
    int  type; /* 0=key, 1=delay, 2=special */
    char data[64];
    int  delay_ms;
} InputEvent;

typedef struct {
    InputEvent events[IREC_MAX_EVENTS];
    int  count;
    int  recording;
    int  playing;
    int  play_index;
    int  repeat_count;
    int  speed_multiplier; /* 100 = normal, 200 = 2x, 50 = 0.5x */
    char name[64];
    unsigned long start_time;
} InputRecorder;

void irec_init(InputRecorder *ir);
void irec_start_recording(InputRecorder *ir, const char *name);
void irec_stop_recording(InputRecorder *ir);
int  irec_add_key(InputRecorder *ir, const char *key, int delay_ms);
int  irec_start_playback(InputRecorder *ir, int repeat);
int  irec_next_event(InputRecorder *ir, InputEvent *out);
int  irec_is_recording(InputRecorder *ir);
int  irec_is_playing(InputRecorder *ir);
void irec_stop_playback(InputRecorder *ir);
int  irec_save(InputRecorder *ir, const char *path);
int  irec_load(InputRecorder *ir, const char *path);

#endif
