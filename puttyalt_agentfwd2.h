#ifndef PUTTYALT_AGENTFWD2_H
#define PUTTYALT_AGENTFWD2_H

#define AGENT_MAX_KEYS  32

typedef enum {
    AGENT_FWD_DISABLED = 0,
    AGENT_FWD_CONFIRM,    /* ask before each use */
    AGENT_FWD_ALLOW,      /* always allow */
    AGENT_FWD_ALLOW_LISTED /* allow only listed keys */
} AgentFwdPolicy;

typedef struct AgentKeyInfo {
    char fingerprint[128];
    char comment[256];
    int allowed;
    int use_count;
} AgentKeyInfo;

typedef struct AgentForwarder {
    AgentFwdPolicy policy;
    AgentKeyInfo keys[AGENT_MAX_KEYS];
    int num_keys;
    int forwarding_active;
    unsigned long total_uses;
} AgentForwarder;

void agentfwd_init(AgentForwarder *af);
void agentfwd_set_policy(AgentForwarder *af, AgentFwdPolicy policy);
int  agentfwd_add_key(AgentForwarder *af, const char *fingerprint,
                      const char *comment, int allowed);
int  agentfwd_check(AgentForwarder *af, const char *fingerprint);
int  agentfwd_remove_key(AgentForwarder *af, const char *fingerprint);

#endif
