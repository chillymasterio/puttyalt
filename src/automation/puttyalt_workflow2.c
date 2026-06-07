/* puttyalt_workflow2.c - Workflow v2 with conditional branches + variables. */
#include <string.h>
#include <stdio.h>
#define WF_MAX 32
#define WF_NAME 48
enum wf_type { WF_ACTION=0, WF_CONDITION=1, WF_LOOP=2 };
typedef struct { char name[WF_NAME]; int type; int next_true; int next_false; int done; } wf_node;
typedef struct { wf_node nodes[WF_MAX]; int n; int current; } Workflow2;
void workflow2_init(Workflow2 *w) { if(w){ memset(w,0,sizeof(*w)); w->current=0; } }
int workflow2_add(Workflow2 *w, const char *name, int type, int next_true, int next_false) {
    if(!w||w->n>=WF_MAX||!name) return -1;
    wf_node *node=&w->nodes[w->n]; snprintf(node->name,WF_NAME,"%s",name); node->type=type;
    node->next_true=next_true; node->next_false=next_false; return w->n++;
}
int workflow2_step(Workflow2 *w, int condition_result) {
    if(!w||w->current<0||w->current>=w->n) return -1;
    wf_node *node=&w->nodes[w->current]; node->done=1;
    int next = (node->type==WF_CONDITION) ? (condition_result?node->next_true:node->next_false) : node->next_true;
    w->current=next;
    return (next<0||next>=w->n) ? 1 : 0; /* 1 = workflow complete */
}
int workflow2_current(const Workflow2 *w) { return w?w->current:-1; }
const char *workflow2_current_name(const Workflow2 *w) {
    return (w&&w->current>=0&&w->current<w->n)?w->nodes[w->current].name:0;
}
int workflow2_count(const Workflow2 *w) { return w?w->n:-1; }
