/*
 * puttyalt_notify.c: Notification system implementation.
 */

#include "puttyalt_notify.h"
#include <string.h>
#include <time.h>

void notify_init(NotifyManager *nm)
{
    memset(nm, 0, sizeof(*nm));
    nm->enabled = 1;
    nm->sound_enabled = 1;
    nm->duration_ms = 5000;
}

void notify_push(NotifyManager *nm, NotifyLevel level,
                 const char *title, const char *body)
{
    if (!nm->enabled)
        return;

    int idx = (nm->head + nm->count) % MAX_NOTIFY_QUEUE;
    if (nm->count >= MAX_NOTIFY_QUEUE) {
        /* Ring buffer full, overwrite oldest */
        nm->head = (nm->head + 1) % MAX_NOTIFY_QUEUE;
    } else {
        nm->count++;
    }

    Notification *n = &nm->queue[idx];
    memset(n, 0, sizeof(*n));
    n->level = level;
    n->timestamp = (long)time(NULL);
    if (title)
        snprintf(n->title, sizeof(n->title), "%s", title);
    if (body)
        snprintf(n->body, sizeof(n->body), "%s", body);
}

const Notification *notify_peek(const NotifyManager *nm)
{
    if (nm->count == 0)
        return NULL;
    return &nm->queue[nm->head];
}

void notify_dismiss(NotifyManager *nm)
{
    if (nm->count > 0) {
        nm->queue[nm->head].dismissed = 1;
        nm->head = (nm->head + 1) % MAX_NOTIFY_QUEUE;
        nm->count--;
    }
}

void notify_dismiss_all(NotifyManager *nm)
{
    nm->count = 0;
    nm->head = 0;
}

void notify_set_enabled(NotifyManager *nm, int enabled)
{
    nm->enabled = enabled ? 1 : 0;
}
