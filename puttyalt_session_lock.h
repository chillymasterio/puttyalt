#ifndef PUTTYALT_SESSION_LOCK_H
#define PUTTYALT_SESSION_LOCK_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Session_lockState;

int  session_lock_init(Session_lockState *s);
int  session_lock_update(Session_lockState *s);
void session_lock_destroy(Session_lockState *s);

#endif
