#ifndef PUTTYALT_MULTICLIP_H
#define PUTTYALT_MULTICLIP_H

#define CLIP_SLOTS 10
#define CLIP_MAX_LEN 65536

typedef struct {
    char *data[CLIP_SLOTS];
    int   len[CLIP_SLOTS];
    char  label[CLIP_SLOTS][32];
    unsigned long timestamp[CLIP_SLOTS];
    int   active_slot;
    int   ring_mode; /* auto-rotate slots */
    int   sync_system; /* sync with OS clipboard */
} MultiClipboard;

void mclip_init(MultiClipboard *mc);
void mclip_free(MultiClipboard *mc);
int  mclip_copy(MultiClipboard *mc, int slot, const char *data, int len);
int  mclip_copy_next(MultiClipboard *mc, const char *data, int len);
const char *mclip_paste(MultiClipboard *mc, int slot, int *len);
const char *mclip_paste_active(MultiClipboard *mc, int *len);
void mclip_set_label(MultiClipboard *mc, int slot, const char *label);
void mclip_clear(MultiClipboard *mc, int slot);
void mclip_clear_all(MultiClipboard *mc);

#endif
