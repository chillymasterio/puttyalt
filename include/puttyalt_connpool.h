#ifndef PUTTYALT_CONNPOOL_H
#define PUTTYALT_CONNPOOL_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} ConnpoolState;

int  connpool_init(ConnpoolState *s);
int  connpool_update(ConnpoolState *s);
void connpool_destroy(ConnpoolState *s);

#endif
