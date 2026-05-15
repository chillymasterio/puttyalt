#ifndef PUTTYALT_SESSREPLAY_H
#define PUTTYALT_SESSREPLAY_H
#include <stdint.h>

typedef enum { REPLAY_STOPPED, REPLAY_RECORDING, REPLAY_PLAYING, REPLAY_PAUSED } ReplayState;

typedef struct {
    uint32_t timestamp_ms;
    uint16_t data_len;
    uint8_t *data;
} ReplayFrame;

typedef struct {
    ReplayFrame *frames;
    int frame_count;
    int frame_capacity;
    ReplayState state;
    int play_pos;
    float play_speed;
    uint32_t start_time;
    uint32_t total_duration_ms;
    char filename[512];
} SessionReplay;

void replay_init(SessionReplay *sr);
void replay_destroy(SessionReplay *sr);
int  replay_start_recording(SessionReplay *sr);
void replay_stop_recording(SessionReplay *sr);
int  replay_add_frame(SessionReplay *sr, const uint8_t *data, int len);
int  replay_save(SessionReplay *sr, const char *path);
int  replay_load(SessionReplay *sr, const char *path);
int  replay_play(SessionReplay *sr);
void replay_pause(SessionReplay *sr);
void replay_stop(SessionReplay *sr);
void replay_set_speed(SessionReplay *sr, float speed);
ReplayFrame *replay_next_frame(SessionReplay *sr);

#endif
