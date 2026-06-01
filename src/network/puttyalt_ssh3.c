/* puttyalt_ssh3.c - SSH-over-QUIC (SSH3) connection descriptor + stream multiplex. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SSH3_MAX_STREAMS 64
enum ssh3_stream_type { SSH3_CTRL=0, SSH3_SHELL=1, SSH3_FORWARD=2, SSH3_SFTP=3 };
typedef struct { uint64_t id; int type; int open; uint64_t bytes; } ssh3_stream;
typedef struct {
    char authority[128]; int port; int udp_mtu;
    ssh3_stream streams[SSH3_MAX_STREAMS]; int stream_count;
    uint64_t next_stream_id; int established;
} Ssh3Conn;
void ssh3_init(Ssh3Conn *c, const char *authority, int port) {
    if(!c) return; memset(c,0,sizeof(*c));
    snprintf(c->authority,128,"%s",authority?authority:""); c->port=port>0?port:443;
    c->udp_mtu=1200; c->next_stream_id=1;
}
int ssh3_establish(Ssh3Conn *c) { if(!c||!c->authority[0]) return -1; c->established=1; return 0; }
int64_t ssh3_open_stream(Ssh3Conn *c, int type) {
    if(!c||!c->established||c->stream_count>=SSH3_MAX_STREAMS) return -1;
    ssh3_stream *s=&c->streams[c->stream_count++];
    s->id=c->next_stream_id++; s->type=type; s->open=1; return (int64_t)s->id;
}
int ssh3_close_stream(Ssh3Conn *c, uint64_t id) {
    if(!c) return -1;
    for (int i=0;i<c->stream_count;i++) if (c->streams[i].id==id && c->streams[i].open) { c->streams[i].open=0; return 0; }
    return -1;
}
int ssh3_stream_write(Ssh3Conn *c, uint64_t id, int len) {
    if(!c||len<0) return -1;
    for (int i=0;i<c->stream_count;i++) if (c->streams[i].id==id && c->streams[i].open) { c->streams[i].bytes+=len; return 0; }
    return -1;
}
int ssh3_open_streams(const Ssh3Conn *c) {
    if(!c) return -1; int n=0;
    for (int i=0;i<c->stream_count;i++) if (c->streams[i].open) n++;
    return n;
}
