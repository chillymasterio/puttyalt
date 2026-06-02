/* puttyalt_sftpqueue.c - SFTP transfer queue with progress + retry tracking. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SQ_MAX 64
#define SQ_PATH 256
enum sq_dir { SQ_UPLOAD=0, SQ_DOWNLOAD=1 };
enum sq_state { SQ_PENDING=0, SQ_ACTIVE, SQ_DONE, SQ_FAILED };
typedef struct { char src[SQ_PATH]; char dst[SQ_PATH]; int dir; int state; uint64_t total, done; int retries; } sq_job;
typedef struct { sq_job j[SQ_MAX]; int n; int max_retries; } SftpQueue;
void sftpqueue_init(SftpQueue *q, int max_retries) { if(!q) return; memset(q,0,sizeof(*q)); q->max_retries=max_retries>0?max_retries:3; }
int sftpqueue_add(SftpQueue *q, const char *src, const char *dst, int dir, uint64_t total) {
    if(!q||q->n>=SQ_MAX) return -1;
    sq_job *j=&q->j[q->n]; snprintf(j->src,SQ_PATH,"%s",src?src:""); snprintf(j->dst,SQ_PATH,"%s",dst?dst:"");
    j->dir=dir; j->total=total; j->state=SQ_PENDING; return q->n++;
}
int sftpqueue_next(SftpQueue *q) {
    if(!q) return -1;
    for (int i=0;i<q->n;i++) if (q->j[i].state==SQ_PENDING) { q->j[i].state=SQ_ACTIVE; return i; }
    return -1;
}
int sftpqueue_progress(SftpQueue *q, int idx, uint64_t done) {
    if(!q||idx<0||idx>=q->n) return -1; q->j[idx].done=done;
    if (done>=q->j[idx].total && q->j[idx].total>0) q->j[idx].state=SQ_DONE;
    return 0;
}
int sftpqueue_fail(SftpQueue *q, int idx) {
    if(!q||idx<0||idx>=q->n) return -1;
    if (++q->j[idx].retries>=q->max_retries) { q->j[idx].state=SQ_FAILED; return 1; }
    q->j[idx].state=SQ_PENDING; return 0;
}
int sftpqueue_overall_pct(const SftpQueue *q) {
    if(!q||q->n==0) return 0; uint64_t t=0,d=0;
    for (int i=0;i<q->n;i++) { t+=q->j[i].total; d+=q->j[i].done; }
    return t? (int)((d*100)/t):0;
}
int sftpqueue_pending(const SftpQueue *q) { if(!q) return -1; int n=0; for(int i=0;i<q->n;i++) if(q->j[i].state==SQ_PENDING||q->j[i].state==SQ_ACTIVE)n++; return n; }
