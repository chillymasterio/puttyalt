#ifndef PUTTYALT_SHAREDCLIP_H
#define PUTTYALT_SHAREDCLIP_H

#define CLIP_MAX_ENTRIES  32
#define CLIP_MAX_SIZE     65536

typedef struct ClipEntry {
    char *data;
    int len;
    unsigned long timestamp;
    int from_session;    /* which tab/session */
} ClipEntry;

typedef struct SharedClipboard {
    ClipEntry entries[CLIP_MAX_ENTRIES];
    int count;
    int head;
    int sync_enabled;    /* sync across sessions */
} SharedClipboard;

void sclip_init(SharedClipboard *sc);
void sclip_free(SharedClipboard *sc);
int  sclip_push(SharedClipboard *sc, const char *data, int len, int session);
const ClipEntry *sclip_peek(const SharedClipboard *sc);
const ClipEntry *sclip_get(const SharedClipboard *sc, int index);
int  sclip_clear(SharedClipboard *sc);

#endif
