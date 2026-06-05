/* puttyalt_jobsched.c - Cron-like job scheduler with next-run computation. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define JS_MAX 24
#define JS_NAME 48
typedef struct { char name[JS_NAME]; int interval_sec; uint64_t next_run_ms; int enabled; int run_count; } js_job;
typedef struct { js_job jobs[JS_MAX]; int n; } JobSched;
void jobsched_init(JobSched *j) { if(j) memset(j,0,sizeof(*j)); }
int jobsched_add(JobSched *j, const char *name, int interval_sec, uint64_t now_ms) {
    if(!j||j->n>=JS_MAX||!name||interval_sec<=0) return -1;
    js_job *job=&j->jobs[j->n++]; snprintf(job->name,JS_NAME,"%s",name); job->interval_sec=interval_sec;
    job->next_run_ms=now_ms+(uint64_t)interval_sec*1000; job->enabled=1; return j->n-1;
}
int jobsched_due(JobSched *j, uint64_t now_ms, int *out_indices, int cap) {
    if(!j) return -1; int n=0;
    for (int i=0;i<j->n && n<cap;i++) if (j->jobs[i].enabled && now_ms>=j->jobs[i].next_run_ms) {
        out_indices[n++]=i; j->jobs[i].next_run_ms=now_ms+(uint64_t)j->jobs[i].interval_sec*1000; j->jobs[i].run_count++;
    }
    return n;
}
void jobsched_enable(JobSched *j, int idx, int on) { if(j&&idx>=0&&idx<j->n) j->jobs[idx].enabled=on?1:0; }
int jobsched_run_count(const JobSched *j, int idx) { return (j&&idx>=0&&idx<j->n)?j->jobs[idx].run_count:-1; }
int jobsched_count(const JobSched *j) { return j?j->n:-1; }
