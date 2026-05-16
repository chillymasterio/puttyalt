/*
 * puttyalt_agentfwd.h: SSH agent forwarding status and control.
 *
 * Shows which keys are loaded in the agent and allows selective
 * forwarding per session for security.
 */

#ifndef PUTTYALT_AGENTFWD_H
#define PUTTYALT_AGENTFWD_H

#define MAX_AGENT_KEYS 32

typedef struct AgentKey {
    char fingerprint[128];
    char comment[256];
    int  bits;
    int  forwarded;     /* 1 = forwarded to this session */
} AgentKey;

typedef struct AgentState {
    AgentKey keys[MAX_AGENT_KEYS];
    int count;
    int forwarding_enabled;
    int confirm_on_use;
} AgentState;

void agent_state_init(AgentState *as);
int  agent_state_refresh(AgentState *as);
void agent_set_forwarding(AgentState *as, int enabled);
void agent_set_confirm(AgentState *as, int confirm);

#endif /* PUTTYALT_AGENTFWD_H */
