/*
 * puttyalt_bandwidth.h: Network bandwidth usage monitor.
 */

#ifndef PUTTYALT_BANDWIDTH_H
#define PUTTYALT_BANDWIDTH_H

#define BW_HISTORY_SIZE  60   /* seconds of history */

typedef struct BandwidthSample {
    unsigned long bytes_rx;
    unsigned long bytes_tx;
    unsigned long timestamp;
} BandwidthSample;

typedef struct BandwidthMonitor {
    BandwidthSample history[BW_HISTORY_SIZE];
    int head;
    int count;
    unsigned long total_rx;
    unsigned long total_tx;
    unsigned long session_start;
    unsigned long peak_rx_rate;
    unsigned long peak_tx_rate;
} BandwidthMonitor;

void bw_init(BandwidthMonitor *mon, unsigned long start_time);
void bw_record(BandwidthMonitor *mon, unsigned long rx, unsigned long tx,
               unsigned long timestamp);
unsigned long bw_get_rx_rate(const BandwidthMonitor *mon);
unsigned long bw_get_tx_rate(const BandwidthMonitor *mon);
void bw_format_bytes(unsigned long bytes, char *buf, int buflen);
void bw_format_rate(unsigned long bytes_per_sec, char *buf, int buflen);

#endif /* PUTTYALT_BANDWIDTH_H */
