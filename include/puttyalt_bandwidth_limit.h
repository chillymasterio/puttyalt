#ifndef PUTTYALT_BANDWIDTH_LIMIT_H
#define PUTTYALT_BANDWIDTH_LIMIT_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Bandwidth_limitState;

int  bandwidth_limit_init(Bandwidth_limitState *s);
int  bandwidth_limit_update(Bandwidth_limitState *s);
void bandwidth_limit_destroy(Bandwidth_limitState *s);

#endif
