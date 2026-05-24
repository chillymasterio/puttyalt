#include <stdio.h>
#include "puttyalt_sftpsync.h"
#include <string.h>
#include <time.h>

void sftpsync_init(SFTPSync *ss)
{
    memset(ss, 0, sizeof(*ss));
    ss->poll_interval_sec = 5;
}

int sftpsync_add_watch(SFTPSync *ss, const char *local, const char *remote)
{
    if (ss->count >= SYNC_MAX_WATCHES) return -1;
    SyncWatch *w = &ss->watches[ss->count++];
    memset(w, 0, sizeof(*w));
    snprintf(w->local_path, sizeof(w->local_path), "%s", local);
    snprintf(w->remote_path, sizeof(w->remote_path), "%s", remote);
    w->recursive = 1;
    return ss->count - 1;
}

int sftpsync_remove_watch(SFTPSync *ss, int index)
{
    if (index < 0 || index >= ss->count) return -1;
    ss->watches[index].active = 0;
    memmove(&ss->watches[index], &ss->watches[index+1], (ss->count-index-1)*sizeof(SyncWatch));
    ss->count--;
    return 0;
}

int sftpsync_start(SFTPSync *ss, int index)
{
    if (index < 0 || index >= ss->count) return -1;
    ss->watches[index].active = 1;
    ss->state = SYNC_WATCHING;
    return 0;
}

int sftpsync_stop(SFTPSync *ss, int index)
{
    if (index < 0 || index >= ss->count) return -1;
    ss->watches[index].active = 0;
    return 0;
}

void sftpsync_stop_all(SFTPSync *ss)
{
    for (int i = 0; i < ss->count; i++) ss->watches[i].active = 0;
    ss->state = SYNC_IDLE;
}

int sftpsync_check(SFTPSync *ss, int index)
{
    if (index < 0 || index >= ss->count || !ss->watches[index].active) return 0;
    /* platform-specific: would stat local files for mtime changes */
    ss->watches[index].last_sync = (unsigned long)time(NULL);
    return 0;
}

SyncWatch *sftpsync_get(SFTPSync *ss, int index)
{
    return (index >= 0 && index < ss->count) ? &ss->watches[index] : NULL;
}
