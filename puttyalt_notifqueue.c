#include "puttyalt_notifqueue.h"
#include <string.h>
#include <time.h>

void notifq_init(NotifQueue *nq)
{
    memset(nq, 0, sizeof(*nq));
    nq->enabled = 1;
    nq->sound_enabled = 1;
    nq->popup_duration_ms = 5000;
    nq->max_visible = 3;
}

int notifq_push(NotifQueue *nq, const char *title, const char *msg, int level)
{
    return notifq_push_action(nq, title, msg, level, -1);
}

int notifq_push_action(NotifQueue *nq, const char *title, const char *msg, int level, int action)
{
    if (!nq->enabled) return -1;
    /* shift down if full */
    if (nq->count >= NOTIF_MAX) {
        memmove(&nq->items[0], &nq->items[1], (NOTIF_MAX - 1) * sizeof(Notification));
        nq->count = NOTIF_MAX - 1;
    }
    Notification *n = &nq->items[nq->count];
    memset(n, 0, sizeof(*n));
    snprintf(n->title, sizeof(n->title), "%s", title);
    snprintf(n->message, sizeof(n->message), "%s", msg);
    n->level = level;
    n->timestamp = (unsigned long)time(NULL);
    n->action_id = action;
    nq->count++;
    nq->unread++;
    return nq->count - 1;
}

void notifq_mark_read(NotifQueue *nq, int index)
{
    if (index >= 0 && index < nq->count && !nq->items[index].read) {
        nq->items[index].read = 1;
        if (nq->unread > 0) nq->unread--;
    }
}

void notifq_mark_all_read(NotifQueue *nq)
{
    for (int i = 0; i < nq->count; i++) nq->items[i].read = 1;
    nq->unread = 0;
}

void notifq_clear(NotifQueue *nq) { nq->count = 0; nq->unread = 0; }
int notifq_unread_count(NotifQueue *nq) { return nq->unread; }
Notification *notifq_latest(NotifQueue *nq) { return nq->count > 0 ? &nq->items[nq->count - 1] : NULL; }
