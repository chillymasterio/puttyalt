#include "puttyalt_activity.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int activity_init(ActivityFeed *af)
{
    memset(af, 0, sizeof(*af));
    af->enabled = 1;
    return 0;
}

void activity_log(ActivityFeed *af, ActivityType type, const char *user,
                  const char *host, const char *detail)
{
    if (!af->enabled) return;
    if (af->count >= ACTIVITY_MAX) {
        /* Shift oldest out */
        for (int i = 0; i < ACTIVITY_MAX - 1; i++)
            af->events[i] = af->events[i + 1];
        af->count = ACTIVITY_MAX - 1;
    }
    ActivityEvent *e = &af->events[af->count];
    e->type = type;
    e->timestamp = (long)time(NULL);
    if (user) snprintf(e->user, sizeof(e->user), "%s", user);
    if (host) snprintf(e->host, sizeof(e->host), "%s", host);
    if (detail) snprintf(e->detail, sizeof(e->detail), "%s", detail);
    af->count++;
}

int activity_get_recent(const ActivityFeed *af, ActivityEvent *out, int max, int minutes)
{
    long cutoff = (long)time(NULL) - (long)minutes * 60;
    int found = 0;
    for (int i = af->count - 1; i >= 0 && found < max; i--) {
        if (af->events[i].timestamp >= cutoff)
            out[found++] = af->events[i];
    }
    return found;
}

void activity_clear(ActivityFeed *af) { af->count = 0; }
void activity_destroy(ActivityFeed *af) { memset(af, 0, sizeof(*af)); }
