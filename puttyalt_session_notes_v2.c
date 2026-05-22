#include "puttyalt_session_notes_v2.h"
#include <string.h>
#include <stdlib.h>

int session_notes_v2_init(Session_notes_v2State *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int session_notes_v2_update(Session_notes_v2State *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void session_notes_v2_destroy(Session_notes_v2State *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
