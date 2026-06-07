/* puttyalt_quickaction.c - Context-aware quick action menu (right-click suggestions). */
#include <string.h>
#include <stdio.h>
#define QA_MAX 32
#define QA_LABEL 48
typedef struct { char label[QA_LABEL]; int action_id; int context_mask; int priority; } qa_action;
typedef struct { qa_action a[QA_MAX]; int n; } QuickAction;
void quickaction_init(QuickAction *q) { if(q) memset(q,0,sizeof(*q)); }
int quickaction_register(QuickAction *q, const char *label, int action_id, int context_mask, int priority) {
    if(!q||q->n>=QA_MAX||!label) return -1;
    qa_action *a=&q->a[q->n++]; snprintf(a->label,QA_LABEL,"%s",label); a->action_id=action_id; a->context_mask=context_mask; a->priority=priority;
    return 0;
}
int quickaction_for_context(QuickAction *q, int context, int *out_ids, int cap) {
    if(!q) return -1;
    int idx[QA_MAX], cnt=0;
    for (int i=0;i<q->n;i++) if (q->a[i].context_mask & context) idx[cnt++]=i;
    /* sort by priority desc */
    for (int a=0;a<cnt;a++) for (int b=a+1;b<cnt;b++) if (q->a[idx[b]].priority>q->a[idx[a]].priority) { int t=idx[a]; idx[a]=idx[b]; idx[b]=t; }
    int n=cnt<cap?cnt:cap; for(int i=0;i<n;i++) out_ids[i]=q->a[idx[i]].action_id; return n;
}
int quickaction_count(const QuickAction *q) { return q?q->n:-1; }
