/* puttyalt_pipefilter.c - Output pipe filter chain (grep/sed/head transformations). */
#include <string.h>
#include <stdio.h>
#define PF_MAX 8
#define PF_ARG 96
enum pf_op { PF_GREP=0, PF_GREP_V=1, PF_HEAD=2, PF_TAIL=3, PF_UNIQ=4 };
typedef struct { int op; char arg[PF_ARG]; int n_param; } pf_stage;
typedef struct { pf_stage stages[PF_MAX]; int n; int lines_seen; } PipeFilter;
void pipefilter_init(PipeFilter *p) { if(p) memset(p,0,sizeof(*p)); }
int pipefilter_add(PipeFilter *p, int op, const char *arg, int n_param) {
    if(!p||p->n>=PF_MAX) return -1;
    pf_stage *s=&p->stages[p->n++]; s->op=op; snprintf(s->arg,PF_ARG,"%s",arg?arg:""); s->n_param=n_param; return 0;
}
/* Returns 1 if line passes all filters, 0 if filtered out. */
int pipefilter_apply(PipeFilter *p, const char *line) {
    if(!p||!line) return 1;
    p->lines_seen++;
    for (int i=0;i<p->n;i++) {
        pf_stage *s=&p->stages[i];
        switch(s->op) {
            case PF_GREP:   if (!strstr(line,s->arg)) return 0; break;
            case PF_GREP_V: if (strstr(line,s->arg)) return 0; break;
            case PF_HEAD:   if (p->lines_seen>s->n_param) return 0; break;
            default: break;
        }
    }
    return 1;
}
void pipefilter_reset(PipeFilter *p) { if(p) p->lines_seen=0; }
int pipefilter_stage_count(const PipeFilter *p) { return p?p->n:-1; }
