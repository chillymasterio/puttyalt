#ifndef PUTTYALT_IDLE_DETECT_H
#define PUTTYALT_IDLE_DETECT_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Idle_detectState;

int  idle_detect_init(Idle_detectState *s);
int  idle_detect_update(Idle_detectState *s);
void idle_detect_destroy(Idle_detectState *s);

#endif
