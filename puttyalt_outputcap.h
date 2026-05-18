#ifndef PUTTYALT_OUTPUTCAP_H
#define PUTTYALT_OUTPUTCAP_H

#define CAP_IDLE     0
#define CAP_ACTIVE   1
#define CAP_PAUSED   2

typedef struct {
    int  state;
    char path[512];
    int  append_mode;
    int  add_timestamps;
    int  strip_ansi;
    int  max_size_mb;
    unsigned long bytes_captured;
    unsigned long start_time;
    void *file_handle;
} OutputCapture;

void outcap_init(OutputCapture *oc);
int  outcap_start(OutputCapture *oc, const char *path, int append);
int  outcap_write(OutputCapture *oc, const char *data, int len);
int  outcap_pause(OutputCapture *oc);
int  outcap_resume(OutputCapture *oc);
int  outcap_stop(OutputCapture *oc);
int  outcap_is_active(OutputCapture *oc);

#endif
