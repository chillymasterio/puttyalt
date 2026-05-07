#ifndef PUTTYALT_BELLV2_H
#define PUTTYALT_BELLV2_H

#define BELL_WAV_PATH_LEN  512

typedef enum {
    BELL_SYSTEM = 0,
    BELL_VISUAL,
    BELL_TASKBAR,
    BELL_WAV,
    BELL_NOTIFICATION,
    BELL_NONE,
    BELL_VIBRATE       /* for future mobile support */
} BellMode;

typedef struct BellConfig {
    BellMode mode;
    char wav_path[BELL_WAV_PATH_LEN];
    int visual_duration_ms;   /* screen flash duration */
    int suppress_after_ms;    /* rate limit: min ms between bells */
    int visual_color;         /* flash color (RGB packed) */
    unsigned long last_bell;
    int total_bells;
} BellConfig;

void bell_init(BellConfig *bc);
int  bell_trigger(BellConfig *bc, unsigned long now);
void bell_set_mode(BellConfig *bc, BellMode mode);
void bell_set_wav(BellConfig *bc, const char *path);
int  bell_is_suppressed(const BellConfig *bc, unsigned long now);

#endif
