/*
 * puttyalt_notify.h: Desktop notification system.
 *
 * Shows toast notifications for connection events, trigger matches,
 * and file transfer completions.
 */

#ifndef PUTTYALT_NOTIFY_H
#define PUTTYALT_NOTIFY_H

#define MAX_NOTIFY_QUEUE 32

typedef enum {
    NOTIFY_INFO,
    NOTIFY_WARNING,
    NOTIFY_ERROR,
    NOTIFY_SUCCESS
} NotifyLevel;

typedef struct Notification {
    char     title[128];
    char     body[512];
    NotifyLevel level;
    long     timestamp;
    int      shown;
    int      dismissed;
} Notification;

typedef struct NotifyManager {
    Notification queue[MAX_NOTIFY_QUEUE];
    int head;
    int count;
    int enabled;
    int sound_enabled;
    int duration_ms;    /* auto-dismiss after N ms, 0 = manual */
} NotifyManager;

void notify_init(NotifyManager *nm);
void notify_push(NotifyManager *nm, NotifyLevel level,
                 const char *title, const char *body);
const Notification *notify_peek(const NotifyManager *nm);
void notify_dismiss(NotifyManager *nm);
void notify_dismiss_all(NotifyManager *nm);
void notify_set_enabled(NotifyManager *nm, int enabled);

#endif /* PUTTYALT_NOTIFY_H */
