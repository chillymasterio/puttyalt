#ifndef PUTTYALT_SSH_AGENT_V2_H
#define PUTTYALT_SSH_AGENT_V2_H

typedef struct {
    int enabled;
    int initialized;
    void *data;
    int data_size;
} Ssh_agent_v2State;

int  ssh_agent_v2_init(Ssh_agent_v2State *s);
int  ssh_agent_v2_update(Ssh_agent_v2State *s);
void ssh_agent_v2_destroy(Ssh_agent_v2State *s);

#endif
