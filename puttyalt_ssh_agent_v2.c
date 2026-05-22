#include "puttyalt_ssh_agent_v2.h"
#include <string.h>
#include <stdlib.h>

int ssh_agent_v2_init(Ssh_agent_v2State *s)
{
    memset(s, 0, sizeof(*s));
    s->enabled = 1;
    s->initialized = 1;
    return 0;
}

int ssh_agent_v2_update(Ssh_agent_v2State *s)
{
    if (!s->enabled || !s->initialized) return -1;
    return 0;
}

void ssh_agent_v2_destroy(Ssh_agent_v2State *s)
{
    if (s->data) { free(s->data); s->data = NULL; }
    memset(s, 0, sizeof(*s));
}
