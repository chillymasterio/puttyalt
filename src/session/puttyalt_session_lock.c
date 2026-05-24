#include "puttyalt_session_lock.h"
#include <string.h>
#include <stdlib.h>

int session_lock_init(Session_lockState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int session_lock_update(Session_lockState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void session_lock_destroy(Session_lockState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
