#ifndef PUTTYALT_NOTIFQUEUE_H
#define PUTTYALT_NOTIFQUEUE_H

#define NOTIF_MAX   64
#define NOTIF_INFO  0
#define NOTIF_WARN  1
#define NOTIF_ERROR 2
#define NOTIF_SUCCESS 3

typedef struct {
    char title[64];
    char message[256];
    int  level;
    int  read;
    int  persistent;
    unsigned long timestamp;
    int  action_id; /* optional action to trigger on click */
} Notification;

typedef struct {
    Notification items[NOTIF_MAX];
    int count;
    int unread;
    int enabled;
    int sound_enabled;
    int popup_duration_ms;
    int max_visible;
} NotifQueue;

void notifq_init(NotifQueue *nq);
int  notifq_push(NotifQueue *nq, const char *title, const char *msg, int level);
int  notifq_push_action(NotifQueue *nq, const char *title, const char *msg, int level, int action);
void notifq_mark_read(NotifQueue *nq, int index);
void notifq_mark_all_read(NotifQueue *nq);
void notifq_clear(NotifQueue *nq);
int  notifq_unread_count(NotifQueue *nq);
Notification *notifq_latest(NotifQueue *nq);

#endif
