#ifndef PUTTYALT_KEEPALIVE_V2_H
#define PUTTYALT_KEEPALIVE_V2_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Keepalive_v2State;

int  keepalive_v2_init(Keepalive_v2State *s);
int  keepalive_v2_update(Keepalive_v2State *s);
void keepalive_v2_destroy(Keepalive_v2State *s);

#endif
