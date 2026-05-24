#include "puttyalt_notifcenter.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void notifcenter_init(NotifCenter *nc)
{
    memset(nc, 0, sizeof(*nc));
    nc->enabled = 1;
    nc->sound_enabled = 1;
    nc->toast_duration_ms = 5000;
}

int notifcenter_add(NotifCenter *nc, NotifLevel level,
                    const char *title, const char *msg, int session_id)
{
    if (!nc->enabled) return -1;

    Notification *n = &nc->items[nc->ring_pos % NOTIF_MAX];
    memset(n, 0, sizeof(*n));
    if (title) snprintf(n->title, sizeof(n->title), "%s", title);
    if (msg) snprintf(n->message, sizeof(n->message), "%s", msg);
    n->level = level;
    n->timestamp = (long)time(NULL);
    n->session_id = session_id;

    nc->ring_pos++;
    if (nc->count < NOTIF_MAX) nc->count++;
    nc->unread++;

    return 0;
}

int notifcenter_mark_read(NotifCenter *nc, int index)
{
    if (index < 0 || index >= nc->count) return -1;
    int real = (nc->ring_pos - nc->count + index + NOTIF_MAX) % NOTIF_MAX;
    if (!nc->items[real].read) {
        nc->items[real].read = 1;
        nc->unread--;
    }
    return 0;
}

void notifcenter_mark_all_read(NotifCenter *nc)
{
    for (int i = 0; i < NOTIF_MAX; i++)
        nc->items[i].read = 1;
    nc->unread = 0;
}

int notifcenter_get_unread(const NotifCenter *nc)
{
    return nc->unread;
}

void notifcenter_clear(NotifCenter *nc)
{
    memset(nc->items, 0, sizeof(nc->items));
    nc->count = 0;
    nc->ring_pos = 0;
    nc->unread = 0;
}
