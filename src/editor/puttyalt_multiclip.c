#include "puttyalt_multiclip.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mclip_init(MultiClipboard *mc)
{
    memset(mc, 0, sizeof(*mc));
    mc->sync_system = 1;
}

void mclip_free(MultiClipboard *mc)
{
    for (int i = 0; i < CLIP_SLOTS; i++) free(mc->data[i]);
    memset(mc, 0, sizeof(*mc));
}

int mclip_copy(MultiClipboard *mc, int slot, const char *data, int len)
{
    if (slot < 0 || slot >= CLIP_SLOTS || !data) return -1;
    if (len > CLIP_MAX_LEN) len = CLIP_MAX_LEN;
    free(mc->data[slot]);
    mc->data[slot] = (char *)malloc(len + 1);
    if (!mc->data[slot]) return -1;
    memcpy(mc->data[slot], data, len);
    mc->data[slot][len] = '\0';
    mc->len[slot] = len;
    mc->timestamp[slot] = (unsigned long)time(NULL);
    mc->active_slot = slot;
    return 0;
}

int mclip_copy_next(MultiClipboard *mc, const char *data, int len)
{
    int slot = mc->ring_mode ? (mc->active_slot + 1) % CLIP_SLOTS : mc->active_slot;
    return mclip_copy(mc, slot, data, len);
}

const char *mclip_paste(MultiClipboard *mc, int slot, int *len)
{
    if (slot < 0 || slot >= CLIP_SLOTS || !mc->data[slot]) return NULL;
    if (len) *len = mc->len[slot];
    return mc->data[slot];
}

const char *mclip_paste_active(MultiClipboard *mc, int *len) { return mclip_paste(mc, mc->active_slot, len); }
void mclip_set_label(MultiClipboard *mc, int slot, const char *label)
{
    if (slot >= 0 && slot < CLIP_SLOTS) snprintf(mc->label[slot], 32, "%s", label);
}

void mclip_clear(MultiClipboard *mc, int slot)
{
    if (slot >= 0 && slot < CLIP_SLOTS) { free(mc->data[slot]); mc->data[slot] = NULL; mc->len[slot] = 0; }
}

void mclip_clear_all(MultiClipboard *mc) { for (int i = 0; i < CLIP_SLOTS; i++) mclip_clear(mc, i); }
