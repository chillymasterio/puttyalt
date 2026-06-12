/* puttyalt_eventhook.c - Event hook dispatcher with priority + cancellation. */
#include <string.h>
#include <stdio.h>
#define EH_MAX 48
#define EH_NAME 40
typedef struct { char event[EH_NAME]; int handler_id; int priority; int cancellable; int enabled; } eh_hook;
typedef struct { eh_hook hooks[EH_MAX]; int n; } EventHook;
void eventhook_init(EventHook *e) { if(e) memset(e,0,sizeof(*e)); }
int eventhook_register(EventHook *e, const char *event, int handler_id, int priority, int cancellable) {
    if(!e||e->n>=EH_MAX||!event) return -1;
    eh_hook *h=&e->hooks[e->n++]; snprintf(h->event,EH_NAME,"%s",event); h->handler_id=handler_id;
    h->priority=priority; h->cancellable=cancellable?1:0; h->enabled=1; return 0;
}
int eventhook_dispatch(EventHook *e, const char *event, int *out_handlers, int cap) {
    if(!e||!event) return -1;
    int idx[EH_MAX], cnt=0;
    for (int i=0;i<e->n;i++) if (e->hooks[i].enabled && strcmp(e->hooks[i].event,event)==0) idx[cnt++]=i;
    /* sort priority desc */
    for (int a=0;a<cnt;a++) for (int b=a+1;b<cnt;b++) if (e->hooks[idx[b]].priority>e->hooks[idx[a]].priority) { int t=idx[a]; idx[a]=idx[b]; idx[b]=t; }
    int n=cnt<cap?cnt:cap; for(int i=0;i<n;i++) out_handlers[i]=e->hooks[idx[i]].handler_id;
    return n;
}
void eventhook_enable(EventHook *e, int handler_id, int on) {
    if(!e) return;
    for(int i=0;i<e->n;i++) if(e->hooks[i].handler_id==handler_id) e->hooks[i].enabled=on?1:0;
}
int eventhook_count(const EventHook *e) { return e?e->n:-1; }
