#include "puttyalt_clipmgr.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void clipmgr_init(ClipManager *cm)
{
    memset(cm, 0, sizeof(*cm));
    cm->max_entries = CLIP_MAX_ENTRIES;
    cm->strip_trailing_whitespace = 1;
    cm->deduplicate = 1;
}

int clipmgr_add(ClipManager *cm, const char *text, const char *source)
{
    if (!text || text[0] == '\0') return -1;
    int len = (int)strlen(text);
    if (len >= CLIP_MAX_LEN) len = CLIP_MAX_LEN - 1;

    /* Deduplicate: check last 10 entries */
    if (cm->deduplicate) {
        int check = cm->count < 10 ? cm->count : 10;
        int start = (cm->ring_pos - check + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
        for (int i = 0; i < check; i++) {
            int idx = (start + i) % CLIP_MAX_ENTRIES;
            if (strcmp(cm->entries[idx].text, text) == 0) return 0;
        }
    }

    ClipEntry *e = &cm->entries[cm->ring_pos % CLIP_MAX_ENTRIES];
    if (e->pinned) {
        /* Skip pinned entries */
        cm->ring_pos++;
        e = &cm->entries[cm->ring_pos % CLIP_MAX_ENTRIES];
    }

    memset(e, 0, sizeof(*e));
    memcpy(e->text, text, len);
    e->text[len] = '\0';

    if (cm->strip_trailing_whitespace) {
        int l = (int)strlen(e->text);
        while (l > 0 && (e->text[l-1] == ' ' || e->text[l-1] == '\t' ||
               e->text[l-1] == '\n' || e->text[l-1] == '\r'))
            e->text[--l] = '\0';
    }

    e->len = (int)strlen(e->text);
    e->timestamp = (long)time(NULL);
    if (source) snprintf(e->source, sizeof(e->source), "%s", source);

    cm->ring_pos++;
    if (cm->count < CLIP_MAX_ENTRIES) cm->count++;
    return 0;
}

int clipmgr_get(const ClipManager *cm, int index, char *out, int max_len)
{
    if (index < 0 || index >= cm->count) return -1;
    int real = (cm->ring_pos - cm->count + index + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
    snprintf(out, max_len, "%s", cm->entries[real].text);
    return cm->entries[real].len;
}

int clipmgr_search(const ClipManager *cm, const char *query, int *results, int max)
{
    int found = 0;
    for (int i = 0; i < cm->count && found < max; i++) {
        int real = (cm->ring_pos - cm->count + i + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
        if (strstr(cm->entries[real].text, query))
            results[found++] = i;
    }
    return found;
}

int clipmgr_pin(ClipManager *cm, int index, int pinned)
{
    if (index < 0 || index >= cm->count) return -1;
    int real = (cm->ring_pos - cm->count + index + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
    cm->entries[real].pinned = pinned ? 1 : 0;
    return 0;
}

int clipmgr_delete(ClipManager *cm, int index)
{
    if (index < 0 || index >= cm->count) return -1;
    int real = (cm->ring_pos - cm->count + index + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
    memset(&cm->entries[real], 0, sizeof(ClipEntry));
    return 0;
}

void clipmgr_clear(ClipManager *cm)
{
    /* Preserve pinned entries */
    for (int i = 0; i < CLIP_MAX_ENTRIES; i++) {
        if (!cm->entries[i].pinned)
            memset(&cm->entries[i], 0, sizeof(ClipEntry));
    }
    cm->count = 0;
    cm->ring_pos = 0;
}
