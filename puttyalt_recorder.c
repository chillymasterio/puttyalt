#include "puttyalt_recorder.h"
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <sys/time.h>
#endif

static void get_time(long *sec, long *usec)
{
#ifndef _WIN32
    struct timeval tv;
    gettimeofday(&tv, NULL);
    *sec = tv.tv_sec;
    *usec = tv.tv_usec;
#else
    *sec = (long)time(NULL);
    *usec = 0;
#endif
}

int recorder_start(SessionRecorder *rec, const char *path)
{
    memset(rec, 0, sizeof(*rec));
    snprintf(rec->path, sizeof(rec->path), "%s", path);
    rec->fp = fopen(path, "wb");
    if (!rec->fp) return -1;
    rec->active = 1;
    rec->start_time = (long)time(NULL);
    return 0;
}

void recorder_stop(SessionRecorder *rec)
{
    if (rec->fp) {
        fclose(rec->fp);
        rec->fp = NULL;
    }
    rec->active = 0;
}

int recorder_write(SessionRecorder *rec, const unsigned char *data, int len)
{
    if (!rec->active || !rec->fp || len <= 0) return -1;

    /* Write ttyrec header: sec(4) + usec(4) + len(4) */
    long sec, usec;
    get_time(&sec, &usec);

    unsigned char header[12];
    header[0] = sec & 0xFF;
    header[1] = (sec >> 8) & 0xFF;
    header[2] = (sec >> 16) & 0xFF;
    header[3] = (sec >> 24) & 0xFF;
    header[4] = usec & 0xFF;
    header[5] = (usec >> 8) & 0xFF;
    header[6] = (usec >> 16) & 0xFF;
    header[7] = (usec >> 24) & 0xFF;
    header[8] = len & 0xFF;
    header[9] = (len >> 8) & 0xFF;
    header[10] = (len >> 16) & 0xFF;
    header[11] = (len >> 24) & 0xFF;

    fwrite(header, 1, 12, rec->fp);
    fwrite(data, 1, len, rec->fp);
    fflush(rec->fp);

    rec->total_bytes += len;
    rec->frame_count++;
    rec->last_write = sec;

    return len;
}

int recorder_is_active(const SessionRecorder *rec)
{
    return rec->active && rec->fp != NULL;
}
