#ifndef PUTTYALT_SFTPMGR_H
#define PUTTYALT_SFTPMGR_H

#define SFTP_MAX_PATH     512
#define SFTP_MAX_ENTRIES   1024
#define SFTP_MAX_QUEUE     64

typedef enum {
    SFTP_FILE = 0,
    SFTP_DIR,
    SFTP_LINK,
    SFTP_SPECIAL
} SFTPFileType;

typedef enum {
    SFTP_XFER_IDLE = 0,
    SFTP_XFER_DOWNLOAD,
    SFTP_XFER_UPLOAD,
    SFTP_XFER_DONE,
    SFTP_XFER_ERROR
} SFTPXferStatus;

typedef struct {
    char          name[256];
    char          path[SFTP_MAX_PATH];
    SFTPFileType  type;
    long          size;
    long          mtime;
    int           permissions;
    char          owner[64];
} SFTPEntry;

typedef struct {
    char            local_path[SFTP_MAX_PATH];
    char            remote_path[SFTP_MAX_PATH];
    SFTPXferStatus  status;
    long            total_bytes;
    long            transferred;
    int             is_upload;
} SFTPTransfer;

typedef struct {
    SFTPEntry    entries[SFTP_MAX_ENTRIES];
    int          entry_count;
    char         cwd[SFTP_MAX_PATH];
    char         local_cwd[SFTP_MAX_PATH];
    SFTPTransfer queue[SFTP_MAX_QUEUE];
    int          queue_count;
    int          queue_active;
    int          connected;
    int          show_hidden;
    int          sort_by;       /* 0=name, 1=size, 2=date, 3=type */
    int          sort_reverse;
} SFTPManager;

void sftp_init(SFTPManager *sm);
int  sftp_list(SFTPManager *sm, const char *path);
int  sftp_chdir(SFTPManager *sm, const char *path);
int  sftp_mkdir(SFTPManager *sm, const char *path);
int  sftp_remove(SFTPManager *sm, const char *path);
int  sftp_rename(SFTPManager *sm, const char *old_name, const char *new_name);
int  sftp_queue_download(SFTPManager *sm, const char *remote, const char *local);
int  sftp_queue_upload(SFTPManager *sm, const char *local, const char *remote);
int  sftp_process_queue(SFTPManager *sm);
void sftp_cancel_transfer(SFTPManager *sm, int index);
void sftp_toggle_hidden(SFTPManager *sm);
void sftp_sort(SFTPManager *sm, int sort_by);

#endif
