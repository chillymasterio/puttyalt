#include <string.h>
#include <stdio.h>
#define NOTIF_MAX 32
#define NOTIF_MSG_LEN 256
typedef enum { NOTIF_INFO, NOTIF_WARN, NOTIF_ERROR, NOTIF_SUCCESS } NotifType;
typedef struct { char message[NOTIF_MSG_LEN]; NotifType type; long timestamp; int read; } Notification;
static Notification g_notifs[NOTIF_MAX]; static int g_notif_count = 0;
void notification_push(const char *msg, NotifType type, long ts) {
    if (g_notif_count >= NOTIF_MAX) { memmove(&g_notifs[0], &g_notifs[1], sizeof(Notification)*(NOTIF_MAX-1)); g_notif_count--; }
    Notification *n = &g_notifs[g_notif_count++]; memset(n, 0, sizeof(*n));
    snprintf(n->message, NOTIF_MSG_LEN, "%s", msg); n->type = type; n->timestamp = ts;
}
int notification_unread(void) { int n=0; for (int i=0;i<g_notif_count;i++) if (!g_notifs[i].read) n++; return n; }
void notification_mark_read(int idx) { if (idx>=0 && idx<g_notif_count) g_notifs[idx].read=1; }
void notification_clear(void) { g_notif_count = 0; }
