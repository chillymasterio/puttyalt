#ifndef PUTTYALT_SPLITVIEW_H
#define PUTTYALT_SPLITVIEW_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} SplitviewState;

int  splitview_init(SplitviewState *s);
int  splitview_update(SplitviewState *s);
void splitview_destroy(SplitviewState *s);

#endif
