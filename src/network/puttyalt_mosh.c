/* puttyalt_mosh.c - Mosh-style roaming UDP session state (SSP datagram model). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MOSH_KEY_LEN 32
enum mosh_state { MOSH_INIT=0, MOSH_HANDSHAKE, MOSH_CONNECTED, MOSH_ROAMING, MOSH_CLOSED };
typedef struct {
    int state; uint64_t local_seq, remote_seq; uint64_t last_recv_ms;
    char peer_ip[64]; int peer_port; int timeout_ms; unsigned char key[MOSH_KEY_LEN];
    uint64_t bytes_sent, bytes_recv;
} MoshSession;
void mosh_init(MoshSession *m, int timeout_ms) {
    if(!m) return;
    memset(m,0,sizeof(*m));
    m->state=MOSH_INIT; m->timeout_ms=timeout_ms>0?timeout_ms:60000;
}
int mosh_set_key(MoshSession *m, const unsigned char *key, int len) {
    if(!m||!key||len!=MOSH_KEY_LEN) return -1;
    memcpy(m->key,key,MOSH_KEY_LEN); m->state=MOSH_HANDSHAKE; return 0;
}
int mosh_connect(MoshSession *m, const char *ip, int port, uint64_t now_ms) {
    if(!m||!ip||m->state<MOSH_HANDSHAKE) return -1;
    snprintf(m->peer_ip,64,"%s",ip); m->peer_port=port; m->state=MOSH_CONNECTED; m->last_recv_ms=now_ms; return 0;
}
int mosh_roam(MoshSession *m, const char *new_ip, int new_port, uint64_t now_ms) {
    if(!m||!new_ip||m->state<MOSH_CONNECTED) return -1;
    snprintf(m->peer_ip,64,"%s",new_ip); m->peer_port=new_port;
    m->state=MOSH_ROAMING; m->last_recv_ms=now_ms; return 0;
}
int mosh_on_datagram(MoshSession *m, uint64_t seq, int len, uint64_t now_ms) {
    if(!m||m->state<MOSH_CONNECTED) return -1;
    if (seq<=m->remote_seq) return 0;
    /* stale/dup */
    m->remote_seq=seq; m->bytes_recv+=len; m->last_recv_ms=now_ms;
    if (m->state==MOSH_ROAMING) m->state=MOSH_CONNECTED;
    return 1;
}
uint64_t mosh_send_seq(MoshSession *m, int len) {
    if(!m) return 0;
    m->bytes_sent+=len; return ++m->local_seq;
}
int mosh_is_timed_out(const MoshSession *m, uint64_t now_ms) {
    if(!m||m->state<MOSH_CONNECTED) return 0;
    return (now_ms - m->last_recv_ms) > (uint64_t)m->timeout_ms ? 1 : 0;
}
int mosh_state(const MoshSession *m) { return m?m->state:-1; }
