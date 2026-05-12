#include "puttyalt_sessreplay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static long now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

int replay_init(SessionReplay *sr, int capacity)
{
    memset(sr, 0, sizeof(*sr));
    if (capacity <= 0) capacity = REPLAY_MAX_FRAMES;
    sr->frames = (ReplayFrame *)calloc(capacity, sizeof(ReplayFrame));
    if (!sr->frames) return -1;
    sr->capacity = capacity;
    sr->speed = 1.0f;
    return 0;
}

void replay_destroy(SessionReplay *sr)
{
    free(sr->frames);
    memset(sr, 0, sizeof(*sr));
}

int replay_record_frame(SessionReplay *sr, const unsigned char *data,
                        int len, int is_input)
{
    if (sr->state != REPLAY_RECORDING) return -1;
    if (sr->frame_count >= sr->capacity) return -1;
    if (len > REPLAY_MAX_DATA) len = REPLAY_MAX_DATA;

    ReplayFrame *f = &sr->frames[sr->frame_count];
    f->timestamp_ms = now_ms() - sr->start_time;
    memcpy(f->data, data, len);
    f->data_len = len;
    f->is_input = is_input;
    sr->frame_count++;
    return 0;
}

int replay_start(SessionReplay *sr)
{
    if (sr->state == REPLAY_RECORDING) {
        sr->start_time = now_ms();
        return 0;
    }
    if (sr->frame_count == 0) return -1;
    sr->state = REPLAY_PLAYING;
    sr->position = 0;
    sr->start_time = now_ms();
    sr->elapsed_ms = 0;
    return 0;
}

int replay_pause(SessionReplay *sr)
{
    if (sr->state == REPLAY_PLAYING) {
        sr->state = REPLAY_PAUSED;
        sr->elapsed_ms += now_ms() - sr->start_time;
        return 0;
    }
    if (sr->state == REPLAY_PAUSED) {
        sr->state = REPLAY_PLAYING;
        sr->start_time = now_ms();
        return 0;
    }
    return -1;
}

int replay_stop(SessionReplay *sr)
{
    sr->state = REPLAY_STOPPED;
    sr->position = 0;
    sr->elapsed_ms = 0;
    return 0;
}

int replay_seek(SessionReplay *sr, int frame)
{
    if (frame < 0) frame = 0;
    if (frame >= sr->frame_count) frame = sr->frame_count - 1;
    sr->position = frame;
    if (frame >= 0 && frame < sr->frame_count)
        sr->elapsed_ms = sr->frames[frame].timestamp_ms;
    return 0;
}

int replay_seek_time(SessionReplay *sr, long ms)
{
    for (int i = 0; i < sr->frame_count; i++) {
        if (sr->frames[i].timestamp_ms >= ms) {
            sr->position = i;
            sr->elapsed_ms = ms;
            return i;
        }
    }
    return -1;
}

void replay_set_speed(SessionReplay *sr, float speed)
{
    if (speed < 0.1f) speed = 0.1f;
    if (speed > 10.0f) speed = 10.0f;
    sr->speed = speed;
}

const ReplayFrame *replay_next_frame(SessionReplay *sr)
{
    if (sr->state != REPLAY_PLAYING || sr->position >= sr->frame_count)
        return NULL;

    long current_ms = sr->elapsed_ms + (long)((now_ms() - sr->start_time) * sr->speed);
    ReplayFrame *f = &sr->frames[sr->position];

    if (f->timestamp_ms <= current_ms) {
        sr->position++;
        if (sr->position >= sr->frame_count && sr->loop) {
            sr->position = 0;
            sr->start_time = now_ms();
            sr->elapsed_ms = 0;
        }
        return f;
    }
    return NULL;
}

int replay_load(SessionReplay *sr, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    int count = 0;
    if (fread(&count, sizeof(int), 1, f) != 1) { fclose(f); return -1; }
    if (count > sr->capacity) count = sr->capacity;
    sr->frame_count = (int)fread(sr->frames, sizeof(ReplayFrame), count, f);
    fclose(f);
    sr->position = 0;
    sr->state = REPLAY_STOPPED;
    return sr->frame_count;
}

int replay_save(const SessionReplay *sr, const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fwrite(&sr->frame_count, sizeof(int), 1, f);
    fwrite(sr->frames, sizeof(ReplayFrame), sr->frame_count, f);
    fclose(f);
    return 0;
}

long replay_duration(const SessionReplay *sr)
{
    if (sr->frame_count == 0) return 0;
    return sr->frames[sr->frame_count - 1].timestamp_ms;
}
