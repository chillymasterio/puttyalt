#ifndef PUTTYALT_SCP_H
#define PUTTYALT_SCP_H

#define SCP_MAX_TRANSFERS  8
#define SCP_PATH_LEN       512

typedef enum {
    SCP_IDLE = 0,
    SCP_UPLOADING,
    SCP_DOWNLOADING,
    SCP_COMPLETE,
    SCP_ERROR
} SCPState;

typedef struct SCPTransfer {
    SCPState state;
    char local_path[SCP_PATH_LEN];
    char remote_path[SCP_PATH_LEN];
    unsigned long total_bytes;
    unsigned long transferred;
    int preserve_attrs;
    int recursive;
    int compression;
    char error_msg[256];
} SCPTransfer;

typedef struct SCPManager {
    SCPTransfer transfers[SCP_MAX_TRANSFERS];
    int count;
    int active;
} SCPManager;

void scp_init(SCPManager *mgr);
int  scp_upload(SCPManager *mgr, const char *local, const char *remote);
int  scp_download(SCPManager *mgr, const char *remote, const char *local);
int  scp_cancel(SCPManager *mgr, int index);
int  scp_progress(const SCPManager *mgr, int index);
void scp_clear_completed(SCPManager *mgr);

#endif
