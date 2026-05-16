/*
 * puttyalt_transfer.c: Transfer progress implementation.
 */

#include "puttyalt_transfer.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void xfer_init(TransferManager *xm)
{
    memset(xm, 0, sizeof(*xm));
}

int xfer_add(TransferManager *xm, const char *local, const char *remote,
             TransferDirection dir, long total_bytes)
{
    if (xm->count >= MAX_TRANSFERS)
        return -1;

    TransferEntry *e = &xm->transfers[xm->count];
    memset(e, 0, sizeof(*e));
    snprintf(e->local_path, sizeof(e->local_path), "%s", local);
    snprintf(e->remote_path, sizeof(e->remote_path), "%s", remote);
    e->direction = dir;
    e->state = XFER_RUNNING;
    e->total_bytes = total_bytes;
    e->start_time = (long)time(NULL);

    xm->active++;
    return xm->count++;
}

void xfer_update(TransferManager *xm, int index, long bytes_done, long elapsed_ms)
{
    if (index < 0 || index >= xm->count)
        return;

    TransferEntry *e = &xm->transfers[index];
    e->transferred_bytes = bytes_done;
    e->elapsed_ms = elapsed_ms;

    if (elapsed_ms > 0)
        e->speed_bps = (double)bytes_done / ((double)elapsed_ms / 1000.0);

    if (e->total_bytes > 0)
        e->progress_pct = (int)(100L * bytes_done / e->total_bytes);
}

void xfer_complete(TransferManager *xm, int index)
{
    if (index < 0 || index >= xm->count)
        return;
    xm->transfers[index].state = XFER_COMPLETE;
    xm->transfers[index].progress_pct = 100;
    if (xm->active > 0) xm->active--;
}

void xfer_error(TransferManager *xm, int index, const char *msg)
{
    if (index < 0 || index >= xm->count)
        return;
    xm->transfers[index].state = XFER_ERROR;
    if (msg)
        snprintf(xm->transfers[index].error, 256, "%s", msg);
    if (xm->active > 0) xm->active--;
}

int xfer_overall_progress(const TransferManager *xm)
{
    if (xm->count == 0) return 0;
    long total = 0, done = 0;
    for (int i = 0; i < xm->count; i++) {
        total += xm->transfers[i].total_bytes;
        done += xm->transfers[i].transferred_bytes;
    }
    if (total == 0) return 0;
    return (int)(100L * done / total);
}

void xfer_format_speed(double bps, char *buf, int bufsize)
{
    if (bps >= 1073741824.0)
        snprintf(buf, bufsize, "%.1f GB/s", bps / 1073741824.0);
    else if (bps >= 1048576.0)
        snprintf(buf, bufsize, "%.1f MB/s", bps / 1048576.0);
    else if (bps >= 1024.0)
        snprintf(buf, bufsize, "%.1f KB/s", bps / 1024.0);
    else
        snprintf(buf, bufsize, "%.0f B/s", bps);
}

void xfer_format_eta(long remaining_bytes, double bps, char *buf, int bufsize)
{
    if (bps <= 0) {
        snprintf(buf, bufsize, "--:--");
        return;
    }
    long seconds = (long)((double)remaining_bytes / bps);
    if (seconds >= 3600)
        snprintf(buf, bufsize, "%ldh%02ldm", seconds / 3600, (seconds % 3600) / 60);
    else if (seconds >= 60)
        snprintf(buf, bufsize, "%ldm%02lds", seconds / 60, seconds % 60);
    else
        snprintf(buf, bufsize, "%lds", seconds);
}
