/* puttyalt_predict.c - Next-command predictor via Markov bigram model. */
#include <string.h>
#include <stdio.h>
#define PR_MAX 64
#define PR_CMD 48
typedef struct { char from[PR_CMD]; char to[PR_CMD]; int count; } pr_edge;
typedef struct { pr_edge edges[PR_MAX]; int n; char last[PR_CMD]; } Predict;
void predict_init(Predict *p) { if(p) memset(p,0,sizeof(*p)); }
int predict_record(Predict *p, const char *cmd) {
    if(!p||!cmd) return -1;
    if (p->last[0]) {
        for (int i=0;i<p->n;i++) if (strcmp(p->edges[i].from,p->last)==0 && strcmp(p->edges[i].to,cmd)==0) {
            p->edges[i].count++; snprintf(p->last,PR_CMD,"%s",cmd); return 0;
        }
        if (p->n<PR_MAX) { pr_edge *e=&p->edges[p->n++]; snprintf(e->from,PR_CMD,"%s",p->last); snprintf(e->to,PR_CMD,"%s",cmd); e->count=1; }
    }
    snprintf(p->last,PR_CMD,"%s",cmd); return 0;
}
int predict_next(const Predict *p, const char *cmd, char *out, int outlen) {
    if(!p||!cmd) return -1;
    int best=-1, bc=0;
    for (int i=0;i<p->n;i++) if (strcmp(p->edges[i].from,cmd)==0 && p->edges[i].count>bc) { bc=p->edges[i].count; best=i; }
    if (best<0) return -1;
    snprintf(out,outlen,"%s",p->edges[best].to); return bc;
}
int predict_edge_count(const Predict *p) { return p?p->n:-1; }
