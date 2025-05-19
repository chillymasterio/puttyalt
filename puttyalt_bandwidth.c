/*
 * puttyalt_bandwidth.c: Network bandwidth usage monitor.
 */

#include <string.h>
#include <stdio.h>
#include "puttyalt_bandwidth.h"

void bw_init(BandwidthMonitor *mon, unsigned long start_time)
{
    memset(mon, 0, sizeof(*mon));
    mon->session_start = start_time;
}

void bw_record(BandwidthMonitor *mon, unsigned long rx, unsigned long tx,
               unsigned long timestamp)
{
    BandwidthSample *s = &mon->history[mon->head];
    s->bytes_rx = rx;
    s->bytes_tx = tx;
    s->timestamp = timestamp;

    mon->total_rx += rx;
    mon->total_tx += tx;

    if (rx > mon->peak_rx_rate)
        mon->peak_rx_rate = rx;
    if (tx > mon->peak_tx_rate)
        mon->peak_tx_rate = tx;

    mon->head = (mon->head + 1) % BW_HISTORY_SIZE;
    if (mon->count < BW_HISTORY_SIZE)
        mon->count++;
}

unsigned long bw_get_rx_rate(const BandwidthMonitor *mon)
{
    if (mon->count < 2)
        return 0;

    int newest = (mon->head - 1 + BW_HISTORY_SIZE) % BW_HISTORY_SIZE;
    int oldest = (mon->head - mon->count + BW_HISTORY_SIZE) % BW_HISTORY_SIZE;

    unsigned long dt = mon->history[newest].timestamp -
                       mon->history[oldest].timestamp;
    if (dt == 0)
        return 0;

    unsigned long total = 0;
    for (int i = 0; i < mon->count; i++) {
        int idx = (oldest + i) % BW_HISTORY_SIZE;
        total += mon->history[idx].bytes_rx;
    }
    return total / dt;
}

unsigned long bw_get_tx_rate(const BandwidthMonitor *mon)
{
    if (mon->count < 2)
        return 0;

    int newest = (mon->head - 1 + BW_HISTORY_SIZE) % BW_HISTORY_SIZE;
    int oldest = (mon->head - mon->count + BW_HISTORY_SIZE) % BW_HISTORY_SIZE;

    unsigned long dt = mon->history[newest].timestamp -
                       mon->history[oldest].timestamp;
    if (dt == 0)
        return 0;

    unsigned long total = 0;
    for (int i = 0; i < mon->count; i++) {
        int idx = (oldest + i) % BW_HISTORY_SIZE;
        total += mon->history[idx].bytes_tx;
    }
    return total / dt;
}

void bw_format_bytes(unsigned long bytes, char *buf, int buflen)
{
    if (bytes >= 1073741824UL)
        snprintf(buf, buflen, "%.2f GB", bytes / 1073741824.0);
    else if (bytes >= 1048576UL)
        snprintf(buf, buflen, "%.2f MB", bytes / 1048576.0);
    else if (bytes >= 1024UL)
        snprintf(buf, buflen, "%.1f KB", bytes / 1024.0);
    else
        snprintf(buf, buflen, "%lu B", bytes);
}

void bw_format_rate(unsigned long bytes_per_sec, char *buf, int buflen)
{
    char tmp[32];
    bw_format_bytes(bytes_per_sec, tmp, sizeof(tmp));
    snprintf(buf, buflen, "%s/s", tmp);
}
