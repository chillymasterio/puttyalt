#ifndef PUTTYALT_ACTIVITY_H
#define PUTTYALT_ACTIVITY_H

#define ACTIVITY_MAX 512

typedef enum {
    ACT_CONNECT, ACT_DISCONNECT, ACT_COMMAND,
    ACT_TRANSFER, ACT_ERROR, ACT_INFO
} ActivityType;

typedef struct {
    ActivityType type;
    long timestamp;
    char user[32];
    char host[64];
    char detail[128];
} ActivityEvent;

typedef struct {
    ActivityEvent events[ACTIVITY_MAX];
    int count;
    int enabled;
} ActivityFeed;

int  activity_init(ActivityFeed *af);
void activity_log(ActivityFeed *af, ActivityType type, const char *user,
                  const char *host, const char *detail);
int  activity_get_recent(const ActivityFeed *af, ActivityEvent *out, int max, int minutes);
void activity_clear(ActivityFeed *af);
void activity_destroy(ActivityFeed *af);

#endif
