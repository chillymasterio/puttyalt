/* puttyalt_muxchan.c - SSH connection multiplexing channel table (ControlMaster-style). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MX_MAX 64
enum mx_type { MX_SESSION=0, MX_DIRECT_TCP=1, MX_X11=2, MX_AGENT=3 };
typedef struct { uint32_t local_id, remote_id; int type; int win_size; int open; uint64_t bytes; } mx_chan;
typedef struct { mx_chan c[MX_MAX]; int n; uint32_t next_id; int master; } MuxChan;
void muxchan_init(MuxChan *m, int is_master) { if(!m) return; memset(m,0,sizeof(*m)); m->next_id=1; m->master=is_master?1:0; }
int64_t muxchan_open(MuxChan *m, int type, int win_size) {
    if(!m||m->n>=MX_MAX) return -1;
    mx_chan *c=&m->c[m->n++]; c->local_id=m->next_id++; c->type=type; c->win_size=win_size>0?win_size:32768; c->open=1;
    return (int64_t)c->local_id;
}
int muxchan_bind_remote(MuxChan *m, uint32_t local_id, uint32_t remote_id) {
    if(!m) return -1;
    for (int i=0;i<m->n;i++) if (m->c[i].local_id==local_id && m->c[i].open) { m->c[i].remote_id=remote_id; return 0; }
    return -1;
}
int muxchan_close(MuxChan *m, uint32_t local_id) {
    if(!m) return -1;
    for (int i=0;i<m->n;i++) if (m->c[i].local_id==local_id && m->c[i].open) { m->c[i].open=0; return 0; }
    return -1;
}
int muxchan_active(const MuxChan *m) { if(!m) return -1; int n=0; for(int i=0;i<m->n;i++) if(m->c[i].open)n++; return n; }
int muxchan_is_master(const MuxChan *m) { return m?m->master:-1; }
