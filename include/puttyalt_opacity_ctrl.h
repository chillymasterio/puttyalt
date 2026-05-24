#ifndef PUTTYALT_OPACITY_CTRL_H
#define PUTTYALT_OPACITY_CTRL_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Opacity_ctrlState;

int  opacity_ctrl_init(Opacity_ctrlState *s);
int  opacity_ctrl_update(Opacity_ctrlState *s);
void opacity_ctrl_destroy(Opacity_ctrlState *s);

#endif
