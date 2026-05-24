#ifndef PUTTYALT_RECONNECT_V2_H
#define PUTTYALT_RECONNECT_V2_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Reconnect_v2State;

int  reconnect_v2_init(Reconnect_v2State *s);
int  reconnect_v2_update(Reconnect_v2State *s);
void reconnect_v2_destroy(Reconnect_v2State *s);

#endif
