/* puttyalt_hookchain.c - Lifecycle hook chain (pre/post connect, disconnect). */
#include <string.h>
#include <stdio.h>
#define HC_MAX 32
#define HC_CMD 160
enum hc_event { HC_PRE_CONNECT=0, HC_POST_CONNECT=1, HC_PRE_DISCONNECT=2, HC_POST_DISCONNECT=3, HC_ON_ERROR=4 };
typedef struct { int event; char command[HC_CMD]; int priority; int enabled; } hc_hook;
typedef struct { hc_hook h[HC_MAX]; int n; } HookChain;
void hookchain_init(HookChain *h) { if(h) memset(h,0,sizeof(*h)); }
int hookchain_register(HookChain *h, int event, const char *command, int priority) {
    if(!h||h->n>=HC_MAX||!command) return -1;
    hc_hook *hk=&h->h[h->n++]; hk->event=event; snprintf(hk->command,HC_CMD,"%s",command); hk->priority=priority; hk->enabled=1;
    return 0;
}
int hookchain_fire(const HookChain *h, int event, int *out_indices, int cap) {
    if(!h) return -1; int n=0;
    /* collect matching enabled hooks */
    int idx[HC_MAX], cnt=0;
    for (int i=0;i<h->n;i++) if (h->h[i].event==event && h->h[i].enabled) idx[cnt++]=i;
    /* sort by priority desc */
    for (int a=0;a<cnt;a++) for (int b=a+1;b<cnt;b++) if (h->h[idx[b]].priority>h->h[idx[a]].priority) { int t=idx[a]; idx[a]=idx[b]; idx[b]=t; }
    for (int i=0;i<cnt && n<cap;i++) out_indices[n++]=idx[i];
    return n;
}
void hookchain_enable(HookChain *h, int idx, int on) { if(h&&idx>=0&&idx<h->n) h->h[idx].enabled=on?1:0; }
int hookchain_count(const HookChain *h) { return h?h->n:-1; }
