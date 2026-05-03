/*
 * puttyalt_transfer.h: File transfer progress tracking.
 *
 * Tracks upload/download progress for the SFTP panel, providing
 * speed calculation, ETA, and completion callbacks.
 */

#ifndef PUTTYALT_TRANSFER_H
#define PUTTYALT_TRANSFER_H

#define MAX_TRANSFERS 32

typedef enum {
    XFER_IDLE,
    XFER_RUNNING,
    XFER_PAUSED,
    XFER_COMPLETE,
    XFER_ERROR
} TransferState;

typedef enum {
    XFER_UPLOAD,
    XFER_DOWNLOAD
} TransferDirection;

typedef struct TransferEntry {
    char              local_path[512];
    char              remote_path[512];
    TransferDirection direction;
    TransferState     state;
    long              total_bytes;
    long              transferred_bytes;
    long              start_time;
    long              elapsed_ms;
    double            speed_bps;       /* bytes per second */
    int               progress_pct;
    char              error[256];
} TransferEntry;

typedef struct TransferManager {
    TransferEntry transfers[MAX_TRANSFERS];
    int count;
    int active;     /* number currently running */
} TransferManager;

void xfer_init(TransferManager *xm);
int  xfer_add(TransferManager *xm, const char *local, const char *remote,
              TransferDirection dir, long total_bytes);
void xfer_update(TransferManager *xm, int index, long bytes_done, long elapsed_ms);
void xfer_complete(TransferManager *xm, int index);
void xfer_error(TransferManager *xm, int index, const char *msg);
int  xfer_overall_progress(const TransferManager *xm);
void xfer_format_speed(double bps, char *buf, int bufsize);
void xfer_format_eta(long remaining_bytes, double bps, char *buf, int bufsize);

#endif /* PUTTYALT_TRANSFER_H */
