#include "puttyalt_quick_actions.h"
#include <string.h>
#include <stdlib.h>

int quick_actions_init(Quick_actionsState *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int quick_actions_update(Quick_actionsState *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void quick_actions_destroy(Quick_actionsState *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
