/* puttyalt_keepalive3.c - Keepalive v3 with adaptive interval + dead detection. */
#include <string.h>
#include <stdint.h>
typedef struct { int base_interval_sec; int current_interval_sec; uint64_t last_sent_ms; uint64_t last_recv_ms; int missed; int max_missed; int dead; } KeepAlive3;
void keepalive3_init(KeepAlive3 *k, int interval_sec, int max_missed, uint64_t now_ms) {
    if(!k) return; memset(k,0,sizeof(*k));
    k->base_interval_sec = interval_sec>0?interval_sec:30; k->current_interval_sec=k->base_interval_sec;
    k->max_missed = max_missed>0?max_missed:3; k->last_recv_ms=now_ms;
}
int keepalive3_due(const KeepAlive3 *k, uint64_t now_ms) {
    if(!k||k->dead) return 0;
    return (now_ms - k->last_sent_ms) >= (uint64_t)k->current_interval_sec*1000 ? 1 : 0;
}
void keepalive3_sent(KeepAlive3 *k, uint64_t now_ms) { if(k){ k->last_sent_ms=now_ms; k->missed++; } }
void keepalive3_recv(KeepAlive3 *k, uint64_t now_ms) {
    if(!k) return; k->last_recv_ms=now_ms; k->missed=0; k->dead=0;
    /* connection healthy -> can relax interval slightly */
    if (k->current_interval_sec < k->base_interval_sec*2) k->current_interval_sec++;
}
int keepalive3_check_dead(KeepAlive3 *k) {
    if(!k) return 0;
    if (k->missed >= k->max_missed) { k->dead=1; return 1; }
    /* network seems unstable -> tighten interval */
    if (k->missed>0 && k->current_interval_sec>k->base_interval_sec/2) k->current_interval_sec--;
    return 0;
}
int keepalive3_is_dead(const KeepAlive3 *k) { return k?k->dead:-1; }
