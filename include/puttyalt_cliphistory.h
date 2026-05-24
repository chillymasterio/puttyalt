/*
 * puttyalt_cliphistory.h: Clipboard history ring buffer.
 *
 * Keeps track of recent copy operations from the terminal so users
 * can paste from previous selections, not just the most recent one.
 */

#ifndef PUTTYALT_CLIPHISTORY_H
#define PUTTYALT_CLIPHISTORY_H

#include <stddef.h>

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

void cliphistory_init(ClipHistory *h);
void cliphistory_free(ClipHistory *h);
void cliphistory_add(ClipHistory *h, const char *text, size_t length,
                     long timestamp);
const ClipEntry *cliphistory_get(const ClipHistory *h, int index);
int cliphistory_search(const ClipHistory *h, const char *query,
                       const ClipEntry **results, int max_results);
void cliphistory_clear(ClipHistory *h);

#endif /* PUTTYALT_CLIPHISTORY_H */
