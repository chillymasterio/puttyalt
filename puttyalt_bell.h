/*
 * puttyalt_bell.h: Custom terminal bell behaviour.
 *
 * Options: system beep, visual flash, taskbar flash, custom WAV,
 * notification, or silence.  Configurable per session.
 */

#ifndef PUTTYALT_BELL_H
#define PUTTYALT_BELL_H

typedef enum {
    BELL_SYSTEM,        /* default OS beep */
    BELL_VISUAL,        /* flash the terminal */
    BELL_TASKBAR,       /* flash taskbar icon */
    BELL_CUSTOM_WAV,    /* play a custom sound */
    BELL_NOTIFICATION,  /* show a desktop notification */
    BELL_NONE           /* silence */
} BellMode;

typedef struct BellConfig {
    BellMode mode;
    char     wav_path[512];
    int      rate_limit_ms;  /* min interval between bells */
    int      flash_duration_ms;
    long     last_bell_time;
} BellConfig;

void bell_config_init(BellConfig *bc);
int  bell_should_fire(BellConfig *bc, long now_ms);
void bell_fire(BellConfig *bc);

#endif /* PUTTYALT_BELL_H */
