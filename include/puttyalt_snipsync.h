/*
 * puttyalt_snipsync.h — Shared snippet library synchronization.
 * v1.4.0 feature: sync snippet libraries via git or network share.
 */

#ifndef PUTTYALT_SNIPSYNC_H
#define PUTTYALT_SNIPSYNC_H

#define SNIPSYNC_MAX_REMOTES  8
#define SNIPSYNC_MAX_PATH     512

typedef enum {
    SNIPSYNC_GIT,
    SNIPSYNC_FILE_SHARE,
    SNIPSYNC_HTTP
} SnipSyncType;

typedef enum {
    SNIPSYNC_OK,
    SNIPSYNC_ERR_CONNECT,
    SNIPSYNC_ERR_CONFLICT,
    SNIPSYNC_ERR_AUTH,
    SNIPSYNC_ERR_IO
} SnipSyncResult;

typedef struct {
    SnipSyncType type;
    char url[SNIPSYNC_MAX_PATH];
    char local_path[SNIPSYNC_MAX_PATH];
    char auth_token[128];
    int auto_sync;
    int sync_interval_sec;
    long last_sync;
} SnipSyncRemote;

typedef struct {
    SnipSyncRemote remotes[SNIPSYNC_MAX_REMOTES];
    int remote_count;
    int enabled;
    int conflict_policy;  /* 0=ask, 1=local wins, 2=remote wins */
} SnipSyncState;

int snipsync_init(SnipSyncState *ss);
int snipsync_add_remote(SnipSyncState *ss, SnipSyncType type,
                        const char *url, const char *auth);
SnipSyncResult snipsync_pull(SnipSyncState *ss, int remote_idx);
SnipSyncResult snipsync_push(SnipSyncState *ss, int remote_idx);
SnipSyncResult snipsync_sync_all(SnipSyncState *ss);
int snipsync_remove_remote(SnipSyncState *ss, int idx);
int snipsync_set_auto(SnipSyncState *ss, int remote_idx, int enabled, int interval);
void snipsync_destroy(SnipSyncState *ss);

#endif /* PUTTYALT_SNIPSYNC_H */
