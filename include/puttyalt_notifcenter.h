#ifndef PUTTYALT_NOTIFCENTER_H
#define PUTTYALT_NOTIFCENTER_H

#define NOTIF_MAX 128
#define NOTIF_MSG_LEN 256

typedef enum {
    NOTIF_INFO,
    NOTIF_SUCCESS,
    NOTIF_WARNING,
    NOTIF_ERROR
} NotifLevel;

typedef struct {
    char title[64];
    char message[NOTIF_MSG_LEN];
    NotifLevel level;
    long timestamp;
    int read;
    int session_id;
} Notification;

typedef struct {
    Notification items[NOTIF_MAX];
    int count;
    int ring_pos;
    int unread;
    int enabled;
    int sound_enabled;
    int toast_duration_ms;
} NotifCenter;

void notifcenter_init(NotifCenter *nc);
int  notifcenter_add(NotifCenter *nc, NotifLevel level,
                     const char *title, const char *msg, int session_id);
int  notifcenter_mark_read(NotifCenter *nc, int index);
void notifcenter_mark_all_read(NotifCenter *nc);
int  notifcenter_get_unread(const NotifCenter *nc);
void notifcenter_clear(NotifCenter *nc);

#endif
