#include "puttyalt_idle_detect.h"
#include <string.h>
#include <stdlib.h>

int idle_detect_init(Idle_detectState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int idle_detect_update(Idle_detectState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void idle_detect_destroy(Idle_detectState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
