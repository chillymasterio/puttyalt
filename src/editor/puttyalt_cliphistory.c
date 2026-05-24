/*
 * puttyalt_cliphistory.c: Terminal clipboard history ring buffer.
 *
 * Keeps track of recent copy operations from the terminal so users
 * can paste from previous selections, not just the most recent one.
 * Accessible via a keyboard shortcut (Ctrl+Shift+V by default).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIP_HISTORY_SIZE 32
#define MAX_CLIP_LENGTH   65536

typedef struct ClipEntry {
    char *text;
    size_t length;
    long timestamp;
} ClipEntry;

typedef struct ClipHistory {
    ClipEntry entries[CLIP_HISTORY_SIZE];
    int head;       /* next write position */
    int count;      /* number of valid entries */
} ClipHistory;

void cliphistory_init(ClipHistory *h)
{
    memset(h, 0, sizeof(*h));
}

void cliphistory_free(ClipHistory *h)
{
    for (int i = 0; i < CLIP_HISTORY_SIZE; i++) {
        free(h->entries[i].text);
        h->entries[i].text = NULL;
    }
    h->count = 0;
    h->head = 0;
}

/*
 * Add a new clipboard entry.  The text is copied internally.
 */
void cliphistory_add(ClipHistory *h, const char *text, size_t length,
                     long timestamp)
{
    if (!text || length == 0)
        return;
    if (length > MAX_CLIP_LENGTH)
        length = MAX_CLIP_LENGTH;

    /* Don't add duplicates of the most recent entry */
    if (h->count > 0) {
        int last = (h->head - 1 + CLIP_HISTORY_SIZE) % CLIP_HISTORY_SIZE;
        if (h->entries[last].text &&
            h->entries[last].length == length &&
            memcmp(h->entries[last].text, text, length) == 0)
            return;
    }

    ClipEntry *e = &h->entries[h->head];
    free(e->text);

    e->text = malloc(length + 1);
    if (!e->text)
        return;

    memcpy(e->text, text, length);
    e->text[length] = '\0';
    e->length = length;
    e->timestamp = timestamp;

    h->head = (h->head + 1) % CLIP_HISTORY_SIZE;
    if (h->count < CLIP_HISTORY_SIZE)
        h->count++;
}

/*
 * Get an entry by index (0 = most recent).
 * Returns NULL if index is out of range.
 */
const ClipEntry *cliphistory_get(const ClipHistory *h, int index)
{
    if (index < 0 || index >= h->count)
        return NULL;

    int pos = (h->head - 1 - index + CLIP_HISTORY_SIZE * 2)
              % CLIP_HISTORY_SIZE;
    return &h->entries[pos];
}

/*
 * Search history for entries containing a substring.
 * Returns the number of matches found.
 */
int cliphistory_search(const ClipHistory *h, const char *query,
                       const ClipEntry **results, int max_results)
{
    int n = 0;
    for (int i = 0; i < h->count && n < max_results; i++) {
        const ClipEntry *e = cliphistory_get(h, i);
        if (e && e->text && strstr(e->text, query))
            results[n++] = e;
    }
    return n;
}

/*
 * Clear all history entries.
 */
void cliphistory_clear(ClipHistory *h)
{
    cliphistory_free(h);
}
