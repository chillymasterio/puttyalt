#ifndef PUTTYALT_BANDWIDTH_H
#define PUTTYALT_BANDWIDTH_H

#define BW_MAX_SAMPLES   300
#define BW_MAX_SESSIONS  32

typedef struct {
    long   tx_bytes[BW_MAX_SAMPLES];
    long   rx_bytes[BW_MAX_SAMPLES];
    int    sample_count;
    int    write_pos;
    long   total_tx;
    long   total_rx;
    long   peak_tx_rate;
    long   peak_rx_rate;
    long   last_sample_time;
    int    session_id;
    int    limit_tx;     /* bytes/sec, 0 = unlimited */
    int    limit_rx;
} BWSession;

typedef struct {
    BWSession sessions[BW_MAX_SESSIONS];
    int       count;
    int       interval_ms;
    int       enabled;
} BWMonitor;

void bw_init(BWMonitor *bw);
int  bw_add_session(BWMonitor *bw, int session_id);
int  bw_remove_session(BWMonitor *bw, int session_id);
void bw_record(BWMonitor *bw, int session_id, long tx, long rx);
long bw_get_tx_rate(const BWMonitor *bw, int session_id);
long bw_get_rx_rate(const BWMonitor *bw, int session_id);
long bw_get_avg_tx(const BWMonitor *bw, int session_id, int samples);
long bw_get_avg_rx(const BWMonitor *bw, int session_id, int samples);
int  bw_set_limit(BWMonitor *bw, int session_id, int tx_limit, int rx_limit);
int  bw_check_limit(const BWMonitor *bw, int session_id, int is_tx);

#endif
