#ifndef PUTTYALT_QUICK_ACTIONS_H
#define PUTTYALT_QUICK_ACTIONS_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Quick_actionsState;

int  quick_actions_init(Quick_actionsState *s);
int  quick_actions_update(Quick_actionsState *s);
void quick_actions_destroy(Quick_actionsState *s);

#endif
