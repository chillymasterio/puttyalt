/* puttyalt_sockstat.c - Per-socket statistics tracker (rtt, retransmits, window). */
#include <string.h>
#include <stdint.h>
typedef struct { uint64_t tx, rx, retrans; int rtt_ms, rtt_min, rtt_max; int cwnd; int samples; } SockStat;
void sockstat_init(SockStat *s) { if(s){ memset(s,0,sizeof(*s)); s->rtt_min=1<<30; } }
void sockstat_on_send(SockStat *s, int bytes) { if(s) s->tx+=bytes; }
void sockstat_on_recv(SockStat *s, int bytes) { if(s) s->rx+=bytes; }
void sockstat_on_retransmit(SockStat *s) { if(s) s->retrans++; }
void sockstat_sample_rtt(SockStat *s, int rtt_ms) {
    if(!s||rtt_ms<0) return;
    s->rtt_ms = s->samples? (s->rtt_ms*7 + rtt_ms)/8 : rtt_ms; /* EWMA */
    if (rtt_ms<s->rtt_min) s->rtt_min=rtt_ms;
    if (rtt_ms>s->rtt_max) s->rtt_max=rtt_ms;
    s->samples++;
}
void sockstat_set_cwnd(SockStat *s, int cwnd) { if(s) s->cwnd=cwnd; }
int sockstat_loss_permille(const SockStat *s) {
    if(!s||s->tx==0) return 0;
    return (int)((s->retrans*1000)/(s->tx? (s->tx/1460+1):1));
}
