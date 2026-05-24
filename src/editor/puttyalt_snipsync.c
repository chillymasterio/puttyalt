/*
 * puttyalt_snipsync.c — Shared snippet library synchronization.
 */

#include "puttyalt_snipsync.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int snipsync_init(SnipSyncState *ss)
{
    memset(ss, 0, sizeof(*ss));
    ss->conflict_policy = 0;  /* ask by default */
    return 0;
}

int snipsync_add_remote(SnipSyncState *ss, SnipSyncType type,
                        const char *url, const char *auth)
{
    if (ss->remote_count >= SNIPSYNC_MAX_REMOTES) return -1;
    if (!url || !url[0]) return -1;

    SnipSyncRemote *r = &ss->remotes[ss->remote_count];
    memset(r, 0, sizeof(*r));
    r->type = type;
    snprintf(r->url, sizeof(r->url), "%s", url);
    if (auth)
        snprintf(r->auth_token, sizeof(r->auth_token), "%s", auth);
    r->sync_interval_sec = 300;  /* 5 min default */
    r->last_sync = 0;

    ss->remote_count++;
    return ss->remote_count - 1;
}

SnipSyncResult snipsync_pull(SnipSyncState *ss, int remote_idx)
{
    if (remote_idx < 0 || remote_idx >= ss->remote_count)
        return SNIPSYNC_ERR_IO;

    SnipSyncRemote *r = &ss->remotes[remote_idx];

    switch (r->type) {
    case SNIPSYNC_GIT:
        /* Execute git pull in snippet directory */
        if (r->local_path[0]) {
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), "git -C \"%s\" pull --rebase 2>&1",
                     r->local_path);
            /* In real impl: execute and parse result */
        }
        break;

    case SNIPSYNC_FILE_SHARE:
        /* Copy files from network share to local */
        break;

    case SNIPSYNC_HTTP:
        /* HTTP GET endpoint for snippet manifest */
        break;
    }

    r->last_sync = (long)time(NULL);
    return SNIPSYNC_OK;
}

SnipSyncResult snipsync_push(SnipSyncState *ss, int remote_idx)
{
    if (remote_idx < 0 || remote_idx >= ss->remote_count)
        return SNIPSYNC_ERR_IO;

    SnipSyncRemote *r = &ss->remotes[remote_idx];

    switch (r->type) {
    case SNIPSYNC_GIT:
        if (r->local_path[0]) {
            char cmd[1024];
            snprintf(cmd, sizeof(cmd),
                     "git -C \"%s\" add -A && git -C \"%s\" commit -m \"sync\" && git -C \"%s\" push 2>&1",
                     r->local_path, r->local_path, r->local_path);
        }
        break;

    case SNIPSYNC_FILE_SHARE:
        /* Copy local snippets to network share */
        break;

    case SNIPSYNC_HTTP:
        /* HTTP POST to sync endpoint */
        break;
    }

    r->last_sync = (long)time(NULL);
    return SNIPSYNC_OK;
}

SnipSyncResult snipsync_sync_all(SnipSyncState *ss)
{
    if (!ss->enabled) return SNIPSYNC_OK;

    SnipSyncResult worst = SNIPSYNC_OK;
    for (int i = 0; i < ss->remote_count; i++) {
        SnipSyncRemote *r = &ss->remotes[i];
        if (!r->auto_sync) continue;

        long now = (long)time(NULL);
        if (now - r->last_sync < r->sync_interval_sec) continue;

        SnipSyncResult res = snipsync_pull(ss, i);
        if (res != SNIPSYNC_OK) worst = res;
        else {
            res = snipsync_push(ss, i);
            if (res != SNIPSYNC_OK) worst = res;
        }
    }
    return worst;
}

int snipsync_remove_remote(SnipSyncState *ss, int idx)
{
    if (idx < 0 || idx >= ss->remote_count) return -1;
    for (int i = idx; i < ss->remote_count - 1; i++)
        ss->remotes[i] = ss->remotes[i + 1];
    ss->remote_count--;
    return 0;
}

int snipsync_set_auto(SnipSyncState *ss, int remote_idx, int enabled, int interval)
{
    if (remote_idx < 0 || remote_idx >= ss->remote_count) return -1;
    ss->remotes[remote_idx].auto_sync = enabled;
    if (interval > 0)
        ss->remotes[remote_idx].sync_interval_sec = interval;
    return 0;
}

void snipsync_destroy(SnipSyncState *ss)
{
    /* Zero auth tokens for security */
    for (int i = 0; i < ss->remote_count; i++) {
        volatile char *p = (volatile char *)ss->remotes[i].auth_token;
        for (int j = 0; j < 128; j++) p[j] = 0;
    }
    memset(ss, 0, sizeof(*ss));
}
