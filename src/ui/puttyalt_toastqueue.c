/* puttyalt_toastqueue.c - Toast notification queue with TTL + max-visible cap. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TQ_MAX 16
#define TQ_TEXT 160
enum toast_kind { TOAST_INFO=0, TOAST_OK, TOAST_WARN, TOAST_ERR };
typedef struct { char text[TQ_TEXT]; int kind; uint64_t shown_ms; int ttl_ms; int active; } toast_item;
typedef struct { toast_item it[TQ_MAX]; int n; int max_visible; } ToastQueue;
void toastqueue_init(ToastQueue *q, int max_visible) {
    if(!q) return; memset(q,0,sizeof(*q)); q->max_visible=max_visible>0?max_visible:3;
}
int toastqueue_push(ToastQueue *q, const char *text, int kind, int ttl_ms, uint64_t now_ms) {
    if(!q||!text) return -1;
    if (q->n>=TQ_MAX) { memmove(&q->it[0],&q->it[1],sizeof(toast_item)*(TQ_MAX-1)); q->n--; }
    toast_item *t=&q->it[q->n++]; snprintf(t->text,TQ_TEXT,"%s",text); t->kind=kind;
    t->ttl_ms=ttl_ms>0?ttl_ms:4000; t->shown_ms=now_ms; t->active=1; return 0;
}
int toastqueue_tick(ToastQueue *q, uint64_t now_ms) {
    if(!q) return -1; int expired=0;
    for (int i=0;i<q->n;i++) if (q->it[i].active && (now_ms-q->it[i].shown_ms)>=(uint64_t)q->it[i].ttl_ms) { q->it[i].active=0; expired++; }
    /* compact inactive */
    int w=0; for (int i=0;i<q->n;i++) if (q->it[i].active) q->it[w++]=q->it[i];
    q->n=w; return expired;
}
int toastqueue_visible(const ToastQueue *q) {
    if(!q) return -1; return q->n<q->max_visible?q->n:q->max_visible;
}
