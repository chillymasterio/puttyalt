/* puttyalt_flowctl.c - Automation flow control (if/while/break state). */
#include <string.h>
#define FC_DEPTH 16
enum fc_block { FC_IF = 0, FC_WHILE = 1, FC_FOR = 2 };
typedef struct {
    int type;
    int condition_met;
    int iteration;
    int max_iterations;
} fc_frame;
typedef struct {
    fc_frame stack[FC_DEPTH];
    int sp;
    int broken;
} FlowCtl;
void flowctl_init(FlowCtl *f) {
    if (f) memset(f, 0, sizeof(*f));
}
int flowctl_enter(FlowCtl *f, int type, int condition, int max_iter) {
    if (!f || f->sp >= FC_DEPTH) return -1;
    fc_frame *fr = &f->stack[f->sp++];
    fr->type = type;
    fr->condition_met = condition ? 1 : 0;
    fr->iteration = 0;
    fr->max_iterations = max_iter > 0 ? max_iter : 1000;
    return 0;
}
int flowctl_should_execute(const FlowCtl *f) {
    if (!f || f->sp == 0 || f->broken) return f && f->sp == 0 && !f->broken;
    return f->stack[f->sp - 1].condition_met;
}
int flowctl_loop_continue(FlowCtl *f, int condition) {
    if (!f || f->sp == 0) return 0;
    fc_frame *fr = &f->stack[f->sp - 1];
    if (fr->type == FC_IF) return 0;
    fr->iteration++;
    if (fr->iteration >= fr->max_iterations) return 0;
    fr->condition_met = condition ? 1 : 0;
    return condition ? 1 : 0;
}
int flowctl_exit(FlowCtl *f) {
    if (!f || f->sp == 0) return -1;
    f->sp--;
    f->broken = 0;
    return 0;
}
void flowctl_break(FlowCtl *f) {
    if (f) f->broken = 1;
}
int flowctl_depth(const FlowCtl *f) {
    return f ? f->sp : -1;
}
