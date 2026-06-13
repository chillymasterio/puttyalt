/* puttyalt_sessrestore2.c - Session restore v2 with window layout persistence. */
#include <string.h>
#include <stdio.h>
#define SR_MAX 24
#define SR_NAME 64
typedef struct { char session_name[SR_NAME]; int window_x, window_y, window_w, window_h; int tab_index; int restored; } sr_entry;
typedef struct { sr_entry entries[SR_MAX]; int n; int enabled; } SessRestore2;
void sessrestore2_init(SessRestore2 *s, int enabled) {
    if(!s) return;
    memset(s,0,sizeof(*s)); s->enabled=enabled?1:0;
}
int sessrestore2_save(SessRestore2 *s, const char *name, int x, int y, int w, int h, int tab) {
    if(!s||s->n>=SR_MAX||!name) return -1;
    sr_entry *e=&s->entries[s->n++]; snprintf(e->session_name,SR_NAME,"%s",name);
    e->window_x=x; e->window_y=y; e->window_w=w; e->window_h=h; e->tab_index=tab; return 0;
}
int sessrestore2_next(SessRestore2 *s) {
    if(!s||!s->enabled) return -1;
    for (int i=0;i<s->n;i++) if (!s->entries[i].restored) { s->entries[i].restored=1; return i; }
    return -1;
}
int sessrestore2_get_geometry(const SessRestore2 *s, int idx, int *x, int *y, int *w, int *h) {
    if(!s||idx<0||idx>=s->n) return -1;
    if(x) *x=s->entries[idx].window_x;
    if(y)*y=s->entries[idx].window_y;
    if(w) *w=s->entries[idx].window_w;
    if(h)*h=s->entries[idx].window_h;
    return 0;
}
int sessrestore2_serialize(const SessRestore2 *s, char *out, int outlen) {
    if(!s||!out) return -1;
    int pos=0;
    for (int i=0;i<s->n && pos<outlen;i++)
        pos+=snprintf(out+pos,outlen-pos,"%s|%d,%d,%d,%d|%d\n",s->entries[i].session_name,
            s->entries[i].window_x,s->entries[i].window_y,s->entries[i].window_w,s->entries[i].window_h,s->entries[i].tab_index);
    return pos;
}
int sessrestore2_count(const SessRestore2 *s) { return s?s->n:-1; }
