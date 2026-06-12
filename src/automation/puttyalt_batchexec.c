/* puttyalt_batchexec.c - Batch execution across multiple sessions. */
#include <string.h>
#include <stdio.h>
#define BE_MAX 32
#define BE_CMD 160
typedef struct { int session_id; int started; int done; int exit_code; } be_target;
typedef struct { be_target targets[BE_MAX]; int n; char command[BE_CMD]; int parallel; int max_parallel; } BatchExec;
void batchexec_init(BatchExec *b, const char *command, int parallel, int max_parallel) {
    if(!b) return;
    memset(b,0,sizeof(*b)); snprintf(b->command,BE_CMD,"%s",command?command:"");
    b->parallel=parallel?1:0; b->max_parallel=max_parallel>0?max_parallel:4;
}
int batchexec_add_target(BatchExec *b, int session_id) {
    if(!b||b->n>=BE_MAX) return -1;
    b->targets[b->n].session_id=session_id; b->targets[b->n].exit_code=-1; return b->n++;
}
int batchexec_next(BatchExec *b) {
    if(!b) return -1;
    int running=0; for(int i=0;i<b->n;i++) if(b->targets[i].started&&!b->targets[i].done) running++;
    if (b->parallel && running>=b->max_parallel) return -1;
    if (!b->parallel && running>0) return -1;
    for (int i=0;i<b->n;i++) if (!b->targets[i].started) { b->targets[i].started=1; return i; }
    return -1;
}
int batchexec_complete(BatchExec *b, int idx, int exit_code) {
    if(!b||idx<0||idx>=b->n) return -1;
    b->targets[idx].done=1; b->targets[idx].exit_code=exit_code; return 0;
}
int batchexec_progress(const BatchExec *b) {
    if(!b||b->n==0) return 0;
    int done=0; for(int i=0;i<b->n;i++) if(b->targets[i].done)done++;
    return done*100/b->n;
}
int batchexec_failures(const BatchExec *b) {
    if(!b) return -1;
    int n=0; for(int i=0;i<b->n;i++) if(b->targets[i].done&&b->targets[i].exit_code!=0)n++; return n;
}
