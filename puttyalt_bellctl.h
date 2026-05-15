#ifndef PUTTYALT_BELLCTL_H
#define PUTTYALT_BELLCTL_H

typedef enum { BELL_NONE, BELL_SYSTEM, BELL_VISUAL, BELL_TASKBAR, BELL_CUSTOM } BellMode;

typedef struct {
    BellMode mode;
    int enabled;
    int flash_duration_ms;
    int cooldown_ms;
    unsigned long last_bell_time;
    int bell_count;
    int muted;
    char custom_sound[512];
    int visual_invert;
    int taskbar_flash;
} BellController;

void bell_init(BellController *bc);
void bell_trigger(BellController *bc);
int  bell_should_fire(BellController *bc);
void bell_mute(BellController *bc);
void bell_unmute(BellController *bc);
void bell_set_mode(BellController *bc, BellMode mode);

#endif
