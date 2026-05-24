#include "puttyalt_bandwidth.h"
#include <string.h>
#include <time.h>

void bw_init(BWMonitor *bw)
{
    memset(bw, 0, sizeof(*bw));
    bw->interval_ms = 1000;
    bw->enabled = 1;
}

static BWSession *bw_find_session(BWMonitor *bw, int session_id)
{
    for (int i = 0; i < bw->count; i++)
        if (bw->sessions[i].session_id == session_id)
            return &bw->sessions[i];
    return NULL;
}

static const BWSession *bw_find_session_const(const BWMonitor *bw, int session_id)
{
    for (int i = 0; i < bw->count; i++)
        if (bw->sessions[i].session_id == session_id)
            return &bw->sessions[i];
    return NULL;
}

int bw_add_session(BWMonitor *bw, int session_id)
{
    if (bw->count >= BW_MAX_SESSIONS) return -1;
    if (bw_find_session(bw, session_id)) return 0;
    BWSession *s = &bw->sessions[bw->count++];
    memset(s, 0, sizeof(*s));
    s->session_id = session_id;
    s->last_sample_time = (long)time(NULL);
    return 0;
}

int bw_remove_session(BWMonitor *bw, int session_id)
{
    for (int i = 0; i < bw->count; i++) {
        if (bw->sessions[i].session_id == session_id) {
            for (int j = i; j < bw->count - 1; j++)
                bw->sessions[j] = bw->sessions[j + 1];
            bw->count--;
            return 0;
        }
    }
    return -1;
}

void bw_record(BWMonitor *bw, int session_id, long tx, long rx)
{
    if (!bw->enabled) return;
    BWSession *s = bw_find_session(bw, session_id);
    if (!s) return;

    s->tx_bytes[s->write_pos] = tx;
    s->rx_bytes[s->write_pos] = rx;
    s->write_pos = (s->write_pos + 1) % BW_MAX_SAMPLES;
    if (s->sample_count < BW_MAX_SAMPLES) s->sample_count++;

    s->total_tx += tx;
    s->total_rx += rx;
    if (tx > s->peak_tx_rate) s->peak_tx_rate = tx;
    if (rx > s->peak_rx_rate) s->peak_rx_rate = rx;
    s->last_sample_time = (long)time(NULL);
}

long bw_get_tx_rate(const BWMonitor *bw, int session_id)
{
    const BWSession *s = bw_find_session_const(bw, session_id);
    if (!s || s->sample_count == 0) return 0;
    int idx = (s->write_pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    return s->tx_bytes[idx];
}

long bw_get_rx_rate(const BWMonitor *bw, int session_id)
{
    const BWSession *s = bw_find_session_const(bw, session_id);
    if (!s || s->sample_count == 0) return 0;
    int idx = (s->write_pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    return s->rx_bytes[idx];
}

long bw_get_avg_tx(const BWMonitor *bw, int session_id, int samples)
{
    const BWSession *s = bw_find_session_const(bw, session_id);
    if (!s || s->sample_count == 0) return 0;
    if (samples > s->sample_count) samples = s->sample_count;
    long sum = 0;
    int pos = (s->write_pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    for (int i = 0; i < samples; i++) {
        sum += s->tx_bytes[pos];
        pos = (pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    }
    return sum / samples;
}

long bw_get_avg_rx(const BWMonitor *bw, int session_id, int samples)
{
    const BWSession *s = bw_find_session_const(bw, session_id);
    if (!s || s->sample_count == 0) return 0;
    if (samples > s->sample_count) samples = s->sample_count;
    long sum = 0;
    int pos = (s->write_pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    for (int i = 0; i < samples; i++) {
        sum += s->rx_bytes[pos];
        pos = (pos - 1 + BW_MAX_SAMPLES) % BW_MAX_SAMPLES;
    }
    return sum / samples;
}

int bw_set_limit(BWMonitor *bw, int session_id, int tx_limit, int rx_limit)
{
    BWSession *s = bw_find_session(bw, session_id);
    if (!s) return -1;
    s->limit_tx = tx_limit;
    s->limit_rx = rx_limit;
    return 0;
}

int bw_check_limit(const BWMonitor *bw, int session_id, int is_tx)
{
    const BWSession *s = bw_find_session_const(bw, session_id);
    if (!s) return 1;
    int limit = is_tx ? s->limit_tx : s->limit_rx;
    if (limit <= 0) return 1; /* unlimited */
    long rate = is_tx ? bw_get_tx_rate(bw, session_id) : bw_get_rx_rate(bw, session_id);
    return rate < limit;
}
