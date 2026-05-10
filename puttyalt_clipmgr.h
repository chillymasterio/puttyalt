#ifndef PUTTYALT_CLIPMGR_H
#define PUTTYALT_CLIPMGR_H

#define CLIP_MAX_ENTRIES 100
#define CLIP_MAX_LEN     8192

typedef struct {
    char text[CLIP_MAX_LEN];
    int len;
    long timestamp;
    int pinned;
    char source[64];  /* which session */
} ClipEntry;

typedef struct {
    ClipEntry entries[CLIP_MAX_ENTRIES];
    int count;
    int ring_pos;
    int sync_enabled;
    int max_entries;
    int strip_trailing_whitespace;
    int deduplicate;
} ClipManager;

void clipmgr_init(ClipManager *cm);
int  clipmgr_add(ClipManager *cm, const char *text, const char *source);
int  clipmgr_get(const ClipManager *cm, int index, char *out, int max_len);
int  clipmgr_search(const ClipManager *cm, const char *query, int *results, int max);
int  clipmgr_pin(ClipManager *cm, int index, int pinned);
int  clipmgr_delete(ClipManager *cm, int index);
void clipmgr_clear(ClipManager *cm);

#endif
