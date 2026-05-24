#include <string.h>
#include "puttyalt_scp.h"

void scp_init(SCPManager *mgr)
{
    memset(mgr, 0, sizeof(*mgr));
}

int scp_upload(SCPManager *mgr, const char *local, const char *remote)
{
    if (mgr->count >= SCP_MAX_TRANSFERS) return -1;
    SCPTransfer *t = &mgr->transfers[mgr->count];
    memset(t, 0, sizeof(*t));
    strncpy(t->local_path, local, SCP_PATH_LEN - 1);
    strncpy(t->remote_path, remote, SCP_PATH_LEN - 1);
    t->state = SCP_UPLOADING;
    t->compression = 1;
    mgr->active++;
    return mgr->count++;
}

int scp_download(SCPManager *mgr, const char *remote, const char *local)
{
    if (mgr->count >= SCP_MAX_TRANSFERS) return -1;
    SCPTransfer *t = &mgr->transfers[mgr->count];
    memset(t, 0, sizeof(*t));
    strncpy(t->local_path, local, SCP_PATH_LEN - 1);
    strncpy(t->remote_path, remote, SCP_PATH_LEN - 1);
    t->state = SCP_DOWNLOADING;
    t->compression = 1;
    mgr->active++;
    return mgr->count++;
}

int scp_cancel(SCPManager *mgr, int index)
{
    if (index < 0 || index >= mgr->count) return -1;
    SCPTransfer *t = &mgr->transfers[index];
    if (t->state == SCP_UPLOADING || t->state == SCP_DOWNLOADING) {
        t->state = SCP_ERROR;
        strncpy(t->error_msg, "Cancelled by user", sizeof(t->error_msg) - 1);
        mgr->active--;
    }
    return 0;
}

int scp_progress(const SCPManager *mgr, int index)
{
    if (index < 0 || index >= mgr->count) return -1;
    const SCPTransfer *t = &mgr->transfers[index];
    if (t->total_bytes == 0) return 0;
    return (int)((t->transferred * 100) / t->total_bytes);
}

void scp_clear_completed(SCPManager *mgr)
{
    int write_idx = 0;
    for (int i = 0; i < mgr->count; i++) {
        if (mgr->transfers[i].state != SCP_COMPLETE &&
            mgr->transfers[i].state != SCP_ERROR) {
            if (write_idx != i)
                mgr->transfers[write_idx] = mgr->transfers[i];
            write_idx++;
        }
    }
    mgr->count = write_idx;
}
