/* puttyalt_webhook2.c - Webhook v2 with retry, signing, and payload templating. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define WH_MAX 16
#define WH_URL 160
typedef struct { char url[WH_URL]; int event_mask; int retries; int max_retries; int delivered; uint32_t secret_hash; } wh_endpoint;
typedef struct { wh_endpoint e[WH_MAX]; int n; } Webhook2;
void webhook2_init(Webhook2 *w) { if(w) memset(w,0,sizeof(*w)); }
int webhook2_register(Webhook2 *w, const char *url, int event_mask, int max_retries) {
    if(!w||w->n>=WH_MAX||!url) return -1;
    wh_endpoint *e=&w->e[w->n]; snprintf(e->url,WH_URL,"%s",url); e->event_mask=event_mask; e->max_retries=max_retries>0?max_retries:3;
    return w->n++;
}
int webhook2_matching(const Webhook2 *w, int event, int *out_idx, int cap) {
    if(!w) return -1; int n=0;
    for (int i=0;i<w->n && n<cap;i++) if (w->e[i].event_mask & (1<<event)) out_idx[n++]=i;
    return n;
}
int webhook2_delivery_result(Webhook2 *w, int idx, int success) {
    if(!w||idx<0||idx>=w->n) return -1;
    if (success) { w->e[idx].delivered++; w->e[idx].retries=0; return 0; }
    if (++w->e[idx].retries>=w->e[idx].max_retries) return -1; /* give up */
    return 1; /* retry */
}
uint32_t webhook2_sign(const char *payload, uint32_t secret) {
    uint32_t h=secret; for (const char *p=payload;*p;p++) { h^=(uint8_t)*p; h*=16777619u; } return h;
}
int webhook2_count(const Webhook2 *w) { return w?w->n:-1; }
