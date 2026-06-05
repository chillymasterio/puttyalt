/* puttyalt_retryqueue.c - Failed-action retry queue with backoff + max attempts. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define RQ_MAX 32
#define RQ_DESC 96
typedef struct { char desc[RQ_DESC]; int attempts; int max_attempts; uint64_t next_retry_ms; int base_ms; } rq_item;
typedef struct { rq_item items[RQ_MAX]; int n; } RetryQueue;
void retryqueue_init(RetryQueue *q) { if(q) memset(q,0,sizeof(*q)); }
int retryqueue_add(RetryQueue *q, const char *desc, int max_attempts, int base_ms, uint64_t now_ms) {
    if(!q||q->n>=RQ_MAX||!desc) return -1;
    rq_item *it=&q->items[q->n++]; snprintf(it->desc,RQ_DESC,"%s",desc);
    it->max_attempts=max_attempts>0?max_attempts:5; it->base_ms=base_ms>0?base_ms:1000;
    it->next_retry_ms=now_ms+it->base_ms; return q->n-1;
}
int retryqueue_due(RetryQueue *q, uint64_t now_ms) {
    if(!q) return -1;
    for (int i=0;i<q->n;i++) if (now_ms>=q->items[i].next_retry_ms) return i;
    return -1;
}
int retryqueue_attempt(RetryQueue *q, int idx, int success, uint64_t now_ms) {
    if(!q||idx<0||idx>=q->n) return -1;
    rq_item *it=&q->items[idx]; it->attempts++;
    if (success) { memmove(&q->items[idx],&q->items[idx+1],sizeof(rq_item)*(q->n-idx-1)); q->n--; return 0; }
    if (it->attempts>=it->max_attempts) { memmove(&q->items[idx],&q->items[idx+1],sizeof(rq_item)*(q->n-idx-1)); q->n--; return -1; }
    int backoff=it->base_ms; for(int k=0;k<it->attempts;k++) backoff*=2;
    it->next_retry_ms=now_ms+backoff; return 1; /* requeued */
}
int retryqueue_size(const RetryQueue *q) { return q?q->n:-1; }
