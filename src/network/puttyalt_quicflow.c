/* puttyalt_quicflow.c - QUIC-style stream flow-control credit accounting. */
#include <stdint.h>
typedef struct { uint64_t max_data; uint64_t sent; uint64_t max_stream_data; uint64_t stream_sent; int blocked; } QuicFlow;
void quicflow_init(QuicFlow *q, uint64_t conn_max, uint64_t stream_max) {
    if(!q) return; q->max_data=conn_max; q->sent=0; q->max_stream_data=stream_max; q->stream_sent=0; q->blocked=0;
}
int quicflow_can_send(const QuicFlow *q, uint64_t n) {
    if(!q) return 0;
    return (q->sent+n<=q->max_data && q->stream_sent+n<=q->max_stream_data)?1:0;
}
int quicflow_send(QuicFlow *q, uint64_t n) {
    if(!q||!quicflow_can_send(q,n)) { if(q)q->blocked=1; return -1; }
    q->sent+=n; q->stream_sent+=n; q->blocked=0; return 0;
}
void quicflow_grant_conn(QuicFlow *q, uint64_t new_max) { if(q&&new_max>q->max_data){ q->max_data=new_max; q->blocked=0; } }
void quicflow_grant_stream(QuicFlow *q, uint64_t new_max) { if(q&&new_max>q->max_stream_data){ q->max_stream_data=new_max; q->blocked=0; } }
uint64_t quicflow_conn_avail(const QuicFlow *q) { return q? (q->max_data-q->sent):0; }
int quicflow_is_blocked(const QuicFlow *q) { return q?q->blocked:-1; }
