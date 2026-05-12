#ifndef PUTTYALT_SESSREPLAY_H
#define PUTTYALT_SESSREPLAY_H

#define REPLAY_MAX_FRAMES   65536
#define REPLAY_MAX_DATA     256

typedef enum {
    REPLAY_STOPPED = 0,
    REPLAY_PLAYING,
    REPLAY_PAUSED,
    REPLAY_RECORDING
} ReplayState;

typedef struct {
    long          timestamp_ms;
    unsigned char data[REPLAY_MAX_DATA];
    int           data_len;
    int           is_input;     /* 0=output, 1=input */
} ReplayFrame;

typedef struct {
    ReplayFrame *frames;
    int          frame_count;
    int          capacity;
    int          position;
    ReplayState  state;
    float        speed;         /* 1.0 = real-time */
    long         start_time;
    long         elapsed_ms;
    int          loop;
} SessionReplay;

int  replay_init(SessionReplay *sr, int capacity);
void replay_destroy(SessionReplay *sr);
int  replay_record_frame(SessionReplay *sr, const unsigned char *data,
                         int len, int is_input);
int  replay_start(SessionReplay *sr);
int  replay_pause(SessionReplay *sr);
int  replay_stop(SessionReplay *sr);
int  replay_seek(SessionReplay *sr, int frame);
int  replay_seek_time(SessionReplay *sr, long ms);
void replay_set_speed(SessionReplay *sr, float speed);
const ReplayFrame *replay_next_frame(SessionReplay *sr);
int  replay_load(SessionReplay *sr, const char *path);
int  replay_save(const SessionReplay *sr, const char *path);
long replay_duration(const SessionReplay *sr);

#endif
