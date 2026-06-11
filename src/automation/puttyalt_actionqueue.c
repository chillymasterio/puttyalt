/* puttyalt_actionqueue.c - Action queue with undo support. */
#include <string.h>
#include <stdio.h>
#define AQ_MAX 64
#define AQ_DESC 64
typedef struct { int action_id; char desc[AQ_DESC]; int undoable; int executed; } aq_action;
typedef struct { aq_action actions[AQ_MAX]; int n; int cursor; } ActionQueue;
void actionqueue_init(ActionQueue *a) { if(a) memset(a,0,sizeof(*a)); }
int actionqueue_push(ActionQueue *a, int action_id, const char *desc, int undoable) {
    if(!a) return -1;
    a->n=a->cursor; /* truncate redo */
    if (a->n>=AQ_MAX) { memmove(&a->actions[0],&a->actions[1],sizeof(aq_action)*(AQ_MAX-1)); a->n--; }
    aq_action *act=&a->actions[a->n++]; act->action_id=action_id; snprintf(act->desc,AQ_DESC,"%s",desc?desc:"");
    act->undoable=undoable?1:0; act->executed=1; a->cursor=a->n; return 0;
}
int actionqueue_can_undo(const ActionQueue *a) { return (a&&a->cursor>0&&a->actions[a->cursor-1].undoable)?1:0; }
int actionqueue_undo(ActionQueue *a) {
    if(!a||a->cursor==0) return -1;
    if (!a->actions[a->cursor-1].undoable) return -1;
    a->cursor--; return a->actions[a->cursor].action_id;
}
int actionqueue_redo(ActionQueue *a) {
    if(!a||a->cursor>=a->n) return -1;
    return a->actions[a->cursor++].action_id;
}
int actionqueue_count(const ActionQueue *a) { return a?a->n:-1; }
