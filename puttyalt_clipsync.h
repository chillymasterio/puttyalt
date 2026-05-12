#ifndef PUTTYALT_CLIPSYNC_H
#define PUTTYALT_CLIPSYNC_H

#define CSYNC_MAX_SESSIONS  32
#define CSYNC_MAX_TEXT      8192
#define CSYNC_MAX_HISTORY   64

typedef struct {
    char text[CSYNC_MAX_TEXT];
    long timestamp;
    int  source_session;
} CSyncEntry;

typedef struct {
    int  session_ids[CSYNC_MAX_SESSIONS];
    int  session_count;
    CSyncEntry history[CSYNC_MAX_HISTORY];
    int  hist_count;
    int  hist_pos;
    char current[CSYNC_MAX_TEXT];
    int  enabled;
    int  auto_sync;
    int  notify_on_copy;
} ClipSync;

void clipsync_init(ClipSync *cs);
int  clipsync_add_session(ClipSync *cs, int session_id);
int  clipsync_remove_session(ClipSync *cs, int session_id);
int  clipsync_push(ClipSync *cs, const char *text, int source_session);
const char *clipsync_get(const ClipSync *cs);
int  clipsync_get_history(const ClipSync *cs, int index, char *buf, int bufsz);
void clipsync_clear_history(ClipSync *cs);
void clipsync_enable(ClipSync *cs, int enable);

#endif
