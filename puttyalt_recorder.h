#ifndef PUTTYALT_RECORDER_H
#define PUTTYALT_RECORDER_H

#include <stdio.h>

typedef struct {
    FILE *fp;
    char path[512];
    int active;
    long start_time;
    long last_write;
    unsigned long total_bytes;
    unsigned long frame_count;
} SessionRecorder;

int  recorder_start(SessionRecorder *rec, const char *path);
void recorder_stop(SessionRecorder *rec);
int  recorder_write(SessionRecorder *rec, const unsigned char *data, int len);
int  recorder_is_active(const SessionRecorder *rec);

#endif
