/* puttyalt_keepalive4.c - Keepalive v4 with RTT-adaptive intervals. */
#include <string.h>
#include <stdint.h>
typedef struct {
    int min_interval_sec;
    int max_interval_sec;
    int current_interval_sec;
    int rtt_ms;
    uint64_t last_sent;
    uint64_t last_recv;
    int missed;
    int dead;
} KeepAlive4;
void keepalive4_init(KeepAlive4 *k, int min_sec, int max_sec, uint64_t now_ms) {
    if (!k) return;
    memset(k, 0, sizeof(*k));
    k->min_interval_sec = min_sec > 0 ? min_sec : 15;
    k->max_interval_sec = max_sec > 0 ? max_sec : 120;
    k->current_interval_sec = k->min_interval_sec;
    k->last_recv = now_ms;
}
void keepalive4_update_rtt(KeepAlive4 *k, int rtt_ms) {
    if (!k) return;
    k->rtt_ms = k->rtt_ms ? (k->rtt_ms * 3 + rtt_ms) / 4 : rtt_ms;
    /* high RTT -> longer interval to reduce overhead */
    if (k->rtt_ms > 300) {
        k->current_interval_sec = k->max_interval_sec;
    } else if (k->rtt_ms < 50) {
        k->current_interval_sec = k->min_interval_sec;
    } else {
        k->current_interval_sec = (k->min_interval_sec + k->max_interval_sec) / 2;
    }
}
int keepalive4_due(const KeepAlive4 *k, uint64_t now_ms) {
    if (!k || k->dead) return 0;
    return (now_ms - k->last_sent) >= (uint64_t)k->current_interval_sec * 1000 ? 1 : 0;
}
void keepalive4_sent(KeepAlive4 *k, uint64_t now_ms) {
    if (!k) return;
    k->last_sent = now_ms;
    k->missed++;
}
void keepalive4_recv(KeepAlive4 *k, uint64_t now_ms) {
    if (!k) return;
    k->last_recv = now_ms;
    k->missed = 0;
    k->dead = 0;
}
int keepalive4_check(KeepAlive4 *k) {
    if (!k) return 0;
    if (k->missed >= 3) {
        k->dead = 1;
        return 1;
    }
    return 0;
}
int keepalive4_interval(const KeepAlive4 *k) {
    return k ? k->current_interval_sec : -1;
}
