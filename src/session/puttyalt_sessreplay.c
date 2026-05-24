#include "puttyalt_sessreplay.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void replay_init(SessionReplay *sr)
{
    memset(sr, 0, sizeof(*sr));
    sr->play_speed = 1.0f;
    sr->frame_capacity = 4096;
    sr->frames = calloc(sr->frame_capacity, sizeof(ReplayFrame));
}

void replay_destroy(SessionReplay *sr)
{
    for (int i = 0; i < sr->frame_count; i++) free(sr->frames[i].data);
    free(sr->frames);
    memset(sr, 0, sizeof(*sr));
}

int replay_start_recording(SessionReplay *sr)
{
    if (sr->state != REPLAY_STOPPED) return -1;
    sr->frame_count = 0;
    sr->start_time = (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
    sr->state = REPLAY_RECORDING;
    return 0;
}

void replay_stop_recording(SessionReplay *sr)
{
    if (sr->state == REPLAY_RECORDING) {
        sr->state = REPLAY_STOPPED;
        sr->total_duration_ms = (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC) - sr->start_time;
    }
}

int replay_add_frame(SessionReplay *sr, const uint8_t *data, int len)
{
    if (sr->state != REPLAY_RECORDING || len <= 0) return -1;
    if (sr->frame_count >= sr->frame_capacity) {
        sr->frame_capacity *= 2;
        sr->frames = realloc(sr->frames, sr->frame_capacity * sizeof(ReplayFrame));
        if (!sr->frames) return -1;
    }
    ReplayFrame *f = &sr->frames[sr->frame_count++];
    f->timestamp_ms = (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC) - sr->start_time;
    f->data_len = (uint16_t)len;
    f->data = malloc(len);
    if (f->data) memcpy(f->data, data, len);
    return 0;
}

int replay_save(SessionReplay *sr, const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fwrite("SREC", 4, 1, f);
    fwrite(&sr->frame_count, sizeof(int), 1, f);
    fwrite(&sr->total_duration_ms, sizeof(uint32_t), 1, f);
    for (int i = 0; i < sr->frame_count; i++) {
        fwrite(&sr->frames[i].timestamp_ms, sizeof(uint32_t), 1, f);
        fwrite(&sr->frames[i].data_len, sizeof(uint16_t), 1, f);
        fwrite(sr->frames[i].data, sr->frames[i].data_len, 1, f);
    }
    fclose(f);
    snprintf(sr->filename, sizeof(sr->filename), "%s", path);
    return 0;
}

int replay_load(SessionReplay *sr, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    char magic[4]; fread(magic, 4, 1, f);
    if (memcmp(magic, "SREC", 4) != 0) { fclose(f); return -1; }
    replay_destroy(sr); replay_init(sr);
    fread(&sr->frame_count, sizeof(int), 1, f);
    fread(&sr->total_duration_ms, sizeof(uint32_t), 1, f);
    if (sr->frame_count > sr->frame_capacity) {
        sr->frame_capacity = sr->frame_count;
        sr->frames = realloc(sr->frames, sr->frame_capacity * sizeof(ReplayFrame));
    }
    for (int i = 0; i < sr->frame_count; i++) {
        fread(&sr->frames[i].timestamp_ms, sizeof(uint32_t), 1, f);
        fread(&sr->frames[i].data_len, sizeof(uint16_t), 1, f);
        sr->frames[i].data = malloc(sr->frames[i].data_len);
        if (sr->frames[i].data) fread(sr->frames[i].data, sr->frames[i].data_len, 1, f);
    }
    fclose(f);
    snprintf(sr->filename, sizeof(sr->filename), "%s", path);
    return 0;
}

int replay_play(SessionReplay *sr) { if (sr->frame_count == 0) return -1; sr->state = REPLAY_PLAYING; sr->play_pos = 0; return 0; }
void replay_pause(SessionReplay *sr) { if (sr->state == REPLAY_PLAYING) sr->state = REPLAY_PAUSED; }
void replay_stop(SessionReplay *sr) { sr->state = REPLAY_STOPPED; sr->play_pos = 0; }
void replay_set_speed(SessionReplay *sr, float speed) { if (speed > 0.0f && speed <= 16.0f) sr->play_speed = speed; }

ReplayFrame *replay_next_frame(SessionReplay *sr)
{
    if (sr->state != REPLAY_PLAYING || sr->play_pos >= sr->frame_count) { replay_stop(sr); return NULL; }
    return &sr->frames[sr->play_pos++];
}
