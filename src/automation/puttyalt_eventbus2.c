/* puttyalt_eventbus2.c - Event bus v2 with topic wildcards + priorities. */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#define EB_MAX 48
#define EB_TOPIC 48
typedef struct { char topic[EB_TOPIC]; int subscriber_id; int priority; int wildcard; } eb_sub;
typedef struct { eb_sub subs[EB_MAX]; int n; uint64_t published; } EventBus2;
void eventbus2_init(EventBus2 *e) { if(e) memset(e,0,sizeof(*e)); }
int eventbus2_subscribe(EventBus2 *e, const char *topic, int subscriber_id, int priority) {
    if(!e||e->n>=EB_MAX||!topic) return -1;
    eb_sub *s=&e->subs[e->n]; snprintf(s->topic,EB_TOPIC,"%s",topic); s->subscriber_id=subscriber_id; s->priority=priority;
    int tl=(int)strlen(topic); s->wildcard=(tl>0 && topic[tl-1]=='*');
    return e->n++;
}
static int eb_match(const eb_sub *s, const char *topic) {
    if (s->wildcard) { int pl=(int)strlen(s->topic)-1; return strncmp(s->topic,topic,pl)==0; }
    return strcmp(s->topic,topic)==0;
}
int eventbus2_publish(EventBus2 *e, const char *topic, int *out_subs, int cap) {
    if(!e||!topic) return -1;
    e->published++;
    int idx[EB_MAX], cnt=0;
    for (int i=0;i<e->n;i++) if (eb_match(&e->subs[i],topic)) idx[cnt++]=i;
    for (int a=0;a<cnt;a++) for (int b=a+1;b<cnt;b++) if (e->subs[idx[b]].priority>e->subs[idx[a]].priority) { int t=idx[a]; idx[a]=idx[b]; idx[b]=t; }
    int n=cnt<cap?cnt:cap; for(int i=0;i<n;i++) out_subs[i]=e->subs[idx[i]].subscriber_id;
    return n;
}
int eventbus2_unsubscribe(EventBus2 *e, int subscriber_id) {
    if(!e) return -1;
    int w=0;
    for (int i=0;i<e->n;i++) if (e->subs[i].subscriber_id!=subscriber_id) e->subs[w++]=e->subs[i];
    int removed=e->n-w; e->n=w; return removed;
}
int eventbus2_count(const EventBus2 *e) { return e?e->n:-1; }
