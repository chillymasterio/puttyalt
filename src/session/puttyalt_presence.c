/*
 * puttyalt_presence.c - presence roster for collaborative shared sessions.
 *
 * Maintains the list of collaborators attached to a shared session along with
 * their online/idle/away/offline status, host flag, join and last-seen times.
 * Heartbeats promote idle peers back online, the tick demotes silent ones,
 * and lines are formatted for the roster sidebar.
 *
 * Self-contained: all types defined below. now_ms supplied by the caller.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRESENCE_MAX        32
#define PRESENCE_NAME_LEN   32
#define PRESENCE_IDLE_MS    30000u    /* online -> idle    after silence */
#define PRESENCE_AWAY_MS    120000u   /* idle   -> away    after silence */
#define PRESENCE_DROP_MS    300000u   /* away   -> offline after silence */

typedef enum {
    PRESENCE_OFFLINE = 0,
    PRESENCE_ONLINE,
    PRESENCE_IDLE,
    PRESENCE_AWAY
} PresenceStatus;

typedef struct {
    uint32_t       user_id;          /* 0 == empty slot */
    char           name[PRESENCE_NAME_LEN];
    PresenceStatus status;
    int            is_host;
    uint32_t       joined_ms;
    uint32_t       last_seen_ms;
} PresenceEntry;

typedef struct {
    PresenceEntry entries[PRESENCE_MAX];
    int           count;
    uint32_t      now_ms;
} PresenceRoster;

void presence_init(PresenceRoster *r)
{
    if (!r) return;
    memset(r, 0, sizeof(*r));
}

PresenceEntry *presence_find(PresenceRoster *r, uint32_t user_id)
{
    int i;
    if (!r || user_id == 0) return NULL;
    for (i = 0; i < r->count; i++)
        if (r->entries[i].user_id == user_id)
            return &r->entries[i];
    return NULL;
}

void presence_tick(PresenceRoster *r, uint32_t now_ms)
{
    int i;
    if (!r) return;
    r->now_ms = now_ms;
    for (i = 0; i < r->count; i++) {
        PresenceEntry *e = &r->entries[i];
        uint32_t silent;
        if (e->user_id == 0 || e->status == PRESENCE_OFFLINE)
            continue;
        silent = (now_ms >= e->last_seen_ms) ? now_ms - e->last_seen_ms : 0;
        if (silent >= PRESENCE_DROP_MS)
            e->status = PRESENCE_OFFLINE;
        else if (silent >= PRESENCE_AWAY_MS)
            e->status = PRESENCE_AWAY;
        else if (silent >= PRESENCE_IDLE_MS && e->status == PRESENCE_ONLINE)
            e->status = PRESENCE_IDLE;
    }
}

int presence_join(PresenceRoster *r, uint32_t user_id,
                  const char *name, int is_host)
{
    PresenceEntry *e;
    if (!r || user_id == 0) return -1;
    e = presence_find(r, user_id);
    if (!e) {
        if (r->count >= PRESENCE_MAX) return -1;
        /* roster full */
        e = &r->entries[r->count++];
        memset(e, 0, sizeof(*e));
        e->user_id = user_id;
        e->joined_ms = r->now_ms;
    }
    if (name) {
        snprintf(e->name, sizeof(e->name), "%s", name);
        e->name[PRESENCE_NAME_LEN - 1] = '\0';
    }
    e->is_host = is_host ? 1 : 0;
    e->status = PRESENCE_ONLINE;
    e->last_seen_ms = r->now_ms;
    return (int)(e - r->entries);
}

int presence_leave(PresenceRoster *r, uint32_t user_id)
{
    int i, j;
    if (!r || user_id == 0) return -1;
    for (i = 0; i < r->count; i++) {
        if (r->entries[i].user_id == user_id) {
            for (j = i; j < r->count - 1; j++)
                r->entries[j] = r->entries[j + 1];
            r->count--;
            memset(&r->entries[r->count], 0, sizeof(PresenceEntry));
            return 0;
        }
    }
    return -1;
}

int presence_heartbeat(PresenceRoster *r, uint32_t user_id)
{
    PresenceEntry *e = presence_find(r, user_id);
    if (!e) return -1;
    e->last_seen_ms = r->now_ms;
    if (e->status != PRESENCE_ONLINE)
        e->status = PRESENCE_ONLINE;   /* promote idle/away/offline back up */
    return 0;
}

int presence_set_status(PresenceRoster *r, uint32_t user_id,
                        PresenceStatus status)
{
    PresenceEntry *e;
    if (status < PRESENCE_OFFLINE || status > PRESENCE_AWAY) return -1;
    e = presence_find(r, user_id);
    if (!e) return -1;
    e->status = status;
    if (status != PRESENCE_OFFLINE)
        e->last_seen_ms = r->now_ms;
    return 0;
}

int presence_online_count(const PresenceRoster *r)
{
    int i, n = 0;
    if (!r) return 0;
    for (i = 0; i < r->count; i++) {
        PresenceStatus s = r->entries[i].status;
        if (r->entries[i].user_id != 0 &&
            (s == PRESENCE_ONLINE || s == PRESENCE_IDLE))
            n++;
    }
    return n;
}

const char *presence_status_label(PresenceStatus s)
{
    switch (s) {
        case PRESENCE_ONLINE:  return "online";
        case PRESENCE_IDLE:    return "idle";
        case PRESENCE_AWAY:    return "away";
        case PRESENCE_OFFLINE: return "offline";
        default:               return "unknown";
    }
}

int presence_format_line(const PresenceRoster *r, int idx,
                         char *buf, int buf_sz)
{
    const PresenceEntry *e;
    int n;
    if (!r || !buf || buf_sz <= 0) return -1;
    if (idx < 0 || idx >= r->count) return -1;
    e = &r->entries[idx];
    if (e->user_id == 0) return -1;
    n = snprintf(buf, (size_t)buf_sz, "%s%s - %s",
                 e->name[0] ? e->name : "(anon)",
                 e->is_host ? " (host)" : "",
                 presence_status_label(e->status));
    if (n < 0 || n >= buf_sz) return -1;
    return n;
}
