#include "puttyalt_sftpmgr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void sftp_init(SFTPManager *sm)
{
    memset(sm, 0, sizeof(*sm));
    snprintf(sm->cwd, SFTP_MAX_PATH, "/");
    snprintf(sm->local_cwd, SFTP_MAX_PATH, ".");
}

int sftp_list(SFTPManager *sm, const char *path)
{
    (void)sm; (void)path;
    /* Real implementation would use libssh2_sftp_opendir/readdir */
    return 0;
}

int sftp_chdir(SFTPManager *sm, const char *path)
{
    if (!path || !*path) return -1;
    if (path[0] == '/')
        snprintf(sm->cwd, SFTP_MAX_PATH, "%s", path);
    else
        snprintf(sm->cwd, SFTP_MAX_PATH, "%s/%s", sm->cwd, path);
    sm->entry_count = 0;
    return sftp_list(sm, sm->cwd);
}

int sftp_mkdir(SFTPManager *sm, const char *path)
{
    (void)sm; (void)path;
    return 0;
}

int sftp_remove(SFTPManager *sm, const char *path)
{
    (void)sm; (void)path;
    return 0;
}

int sftp_rename(SFTPManager *sm, const char *old_name, const char *new_name)
{
    (void)sm; (void)old_name; (void)new_name;
    return 0;
}

int sftp_queue_download(SFTPManager *sm, const char *remote, const char *local)
{
    if (sm->queue_count >= SFTP_MAX_QUEUE) return -1;
    SFTPTransfer *t = &sm->queue[sm->queue_count];
    memset(t, 0, sizeof(*t));
    snprintf(t->remote_path, SFTP_MAX_PATH, "%s", remote);
    snprintf(t->local_path, SFTP_MAX_PATH, "%s", local);
    t->status = SFTP_XFER_IDLE;
    t->is_upload = 0;
    return sm->queue_count++;
}

int sftp_queue_upload(SFTPManager *sm, const char *local, const char *remote)
{
    if (sm->queue_count >= SFTP_MAX_QUEUE) return -1;
    SFTPTransfer *t = &sm->queue[sm->queue_count];
    memset(t, 0, sizeof(*t));
    snprintf(t->local_path, SFTP_MAX_PATH, "%s", local);
    snprintf(t->remote_path, SFTP_MAX_PATH, "%s", remote);
    t->status = SFTP_XFER_IDLE;
    t->is_upload = 1;
    return sm->queue_count++;
}

int sftp_process_queue(SFTPManager *sm)
{
    int processed = 0;
    for (int i = 0; i < sm->queue_count; i++) {
        SFTPTransfer *t = &sm->queue[i];
        if (t->status == SFTP_XFER_IDLE) {
            t->status = t->is_upload ? SFTP_XFER_UPLOAD : SFTP_XFER_DOWNLOAD;
            sm->queue_active = i;
            processed++;
            break;
        }
    }
    return processed;
}

void sftp_cancel_transfer(SFTPManager *sm, int index)
{
    if (index >= 0 && index < sm->queue_count) {
        sm->queue[index].status = SFTP_XFER_ERROR;
    }
}

void sftp_toggle_hidden(SFTPManager *sm)
{
    sm->show_hidden = !sm->show_hidden;
}

static int sftp_cmp_name(const void *a, const void *b)
{
    const SFTPEntry *ea = (const SFTPEntry *)a;
    const SFTPEntry *eb = (const SFTPEntry *)b;
    if (ea->type != eb->type) return ea->type - eb->type;
    return strcmp(ea->name, eb->name);
}

static int sftp_cmp_size(const void *a, const void *b)
{
    const SFTPEntry *ea = (const SFTPEntry *)a;
    const SFTPEntry *eb = (const SFTPEntry *)b;
    return (ea->size > eb->size) - (ea->size < eb->size);
}

static int sftp_cmp_date(const void *a, const void *b)
{
    const SFTPEntry *ea = (const SFTPEntry *)a;
    const SFTPEntry *eb = (const SFTPEntry *)b;
    return (ea->mtime > eb->mtime) - (ea->mtime < eb->mtime);
}

void sftp_sort(SFTPManager *sm, int sort_by)
{
    sm->sort_by = sort_by;
    typedef int (*cmpfn)(const void *, const void *);
    cmpfn fns[] = { sftp_cmp_name, sftp_cmp_size, sftp_cmp_date, sftp_cmp_name };
    int idx = sort_by < 4 ? sort_by : 0;
    qsort(sm->entries, sm->entry_count, sizeof(SFTPEntry), fns[idx]);
}
