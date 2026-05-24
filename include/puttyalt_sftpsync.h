#ifndef PUTTYALT_SFTPSYNC_H
#define PUTTYALT_SFTPSYNC_H

#define SYNC_MAX_WATCHES 16
#define SYNC_IDLE    0
#define SYNC_WATCHING 1
#define SYNC_UPLOADING 2
#define SYNC_ERROR   3

typedef struct {
    char local_path[512];
    char remote_path[512];
    int  active;
    int  recursive;
    int  delete_remote; /* mirror mode: delete remote files not in local */
    char exclude_pattern[128];
    unsigned long last_sync;
    int files_synced;
    int errors;
} SyncWatch;

typedef struct {
    SyncWatch watches[SYNC_MAX_WATCHES];
    int count;
    int state;
    int poll_interval_sec;
    int auto_start;
    unsigned long total_bytes;
    int total_files;
} SFTPSync;

void sftpsync_init(SFTPSync *ss);
int  sftpsync_add_watch(SFTPSync *ss, const char *local, const char *remote);
int  sftpsync_remove_watch(SFTPSync *ss, int index);
int  sftpsync_start(SFTPSync *ss, int index);
int  sftpsync_stop(SFTPSync *ss, int index);
void sftpsync_stop_all(SFTPSync *ss);
int  sftpsync_check(SFTPSync *ss, int index); /* check for changes */
SyncWatch *sftpsync_get(SFTPSync *ss, int index);

#endif
