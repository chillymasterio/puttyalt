/* puttyalt_sshmux.c - SSH session multiplexer: virtual terminals over one connection. */
#include <string.h>
#include <stdio.h>
#define SM_MAX 16
#define SM_NAME 48
typedef struct { char title[SM_NAME]; int channel_id; int active; int rows, cols; } sm_vterm;
typedef struct { sm_vterm v[SM_MAX]; int n; int focused; } SshMux;
void sshmux_init(SshMux *m) { if(m){ memset(m,0,sizeof(*m)); m->focused=-1; } }
int sshmux_open(SshMux *m, const char *title, int channel_id, int rows, int cols) {
    if(!m||m->n>=SM_MAX) return -1;
    sm_vterm *v=&m->v[m->n]; snprintf(v->title,SM_NAME,"%s",title?title:""); v->channel_id=channel_id;
    v->rows=rows; v->cols=cols; v->active=1; if(m->focused<0) m->focused=m->n; return m->n++;
}
int sshmux_close(SshMux *m, int idx) {
    if(!m||idx<0||idx>=m->n) return -1;
    m->v[idx].active=0;
    if (m->focused==idx) { m->focused=-1; for(int i=0;i<m->n;i++) if(m->v[i].active){ m->focused=i; break; } }
    return 0;
}
int sshmux_focus_next(SshMux *m) {
    if(!m||m->n==0) return -1;
    for (int k=1;k<=m->n;k++) { int i=(m->focused+k)%m->n; if (m->v[i].active) { m->focused=i; return i; } }
    return m->focused;
}
int sshmux_focused(const SshMux *m) { return m?m->focused:-1; }
int sshmux_active_count(const SshMux *m) { if(!m) return -1; int n=0; for(int i=0;i<m->n;i++) if(m->v[i].active)n++; return n; }
