/* puttyalt_workspace2.c - Workspace v2: layout + session set + activation order. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define WS2_MAX 12
#define WS2_NAME 48
#define WS2_SESS 16
typedef struct { char name[WS2_NAME]; int sessions[WS2_SESS]; int nsess; int layout_kind; uint8_t last_active; } ws2_space;
typedef struct { ws2_space s[WS2_MAX]; int n; int active; uint8_t clock; } Workspace2;
void workspace2_init(Workspace2 *w) { if(w){ memset(w,0,sizeof(*w)); w->active=-1; } }
int workspace2_create(Workspace2 *w, const char *name, int layout_kind) {
    if(!w||w->n>=WS2_MAX||!name) return -1;
    ws2_space *s=&w->s[w->n]; snprintf(s->name,WS2_NAME,"%s",name); s->layout_kind=layout_kind; return w->n++;
}
int workspace2_add_session(Workspace2 *w, int ws, int session_id) {
    if(!w||ws<0||ws>=w->n||w->s[ws].nsess>=WS2_SESS) return -1;
    w->s[ws].sessions[w->s[ws].nsess++]=session_id; return 0;
}
int workspace2_activate(Workspace2 *w, int ws) {
    if(!w||ws<0||ws>=w->n) return -1;
    w->active=ws; w->s[ws].last_active=++w->clock; return 0;
}
int workspace2_last_used(const Workspace2 *w) {
    if(!w) return -1;
    int best=-1; uint8_t bc=0;
    for (int i=0;i<w->n;i++) if (w->s[i].last_active>=bc) { bc=w->s[i].last_active; best=i; }
    return best;
}
int workspace2_count(const Workspace2 *w) { return w?w->n:-1; }
