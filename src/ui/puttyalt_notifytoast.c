/* puttyalt_notifytoast.c - Desktop notification toast queue with positions. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define NT_MAX 8
#define NT_TEXT 160
enum nt_pos { NT_TOP_RIGHT=0, NT_TOP_LEFT=1, NT_BOTTOM_RIGHT=2, NT_BOTTOM_LEFT=3 };
typedef struct { char title[48]; char body[NT_TEXT]; int sev; uint64_t shown_ms; int ttl_ms; int slot; } nt_toast;
typedef struct { nt_toast toasts[NT_MAX]; int n; int position; int spacing; } NotifyToast;
void notifytoast_init(NotifyToast *n, int position) {
    if(!n) return; memset(n,0,sizeof(*n)); n->position=position; n->spacing=8;
}
int notifytoast_push(NotifyToast *n, const char *title, const char *body, int sev, int ttl_ms, uint64_t now_ms) {
    if(!n||!title) return -1;
    if (n->n>=NT_MAX) { memmove(&n->toasts[0],&n->toasts[1],sizeof(nt_toast)*(NT_MAX-1)); n->n--; }
    nt_toast *t=&n->toasts[n->n]; snprintf(t->title,48,"%s",title); snprintf(t->body,NT_TEXT,"%s",body?body:"");
    t->sev=sev; t->shown_ms=now_ms; t->ttl_ms=ttl_ms>0?ttl_ms:5000; t->slot=n->n; return n->n++;
}
int notifytoast_tick(NotifyToast *n, uint64_t now_ms) {
    if(!n) return -1; int expired=0, w=0;
    for (int i=0;i<n->n;i++) {
        if ((now_ms-n->toasts[i].shown_ms)<(uint64_t)n->toasts[i].ttl_ms) { n->toasts[w]=n->toasts[i]; n->toasts[w].slot=w; w++; }
        else expired++;
    }
    n->n=w; return expired;
}
int notifytoast_slot_y(const NotifyToast *n, int idx, int toast_height) {
    if(!n) return 0; return idx*(toast_height+n->spacing);
}
int notifytoast_count(const NotifyToast *n) { return n?n->n:-1; }
