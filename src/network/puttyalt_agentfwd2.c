#include <string.h>
#include "puttyalt_agentfwd2.h"

void agentfwd_init(AgentForwarder *af)
{
    memset(af, 0, sizeof(*af));
    af->policy = AGENT_FWD_CONFIRM;
}

void agentfwd_set_policy(AgentForwarder *af, AgentFwdPolicy policy)
{
    af->policy = policy;
}

int agentfwd_add_key(AgentForwarder *af, const char *fingerprint,
                     const char *comment, int allowed)
{
    if (af->num_keys >= AGENT_MAX_KEYS) return -1;
    AgentKeyInfo *k = &af->keys[af->num_keys];
    strncpy(k->fingerprint, fingerprint, sizeof(k->fingerprint) - 1);
    if (comment) strncpy(k->comment, comment, sizeof(k->comment) - 1);
    k->allowed = allowed;
    k->use_count = 0;
    af->num_keys++;
    return 0;
}

int agentfwd_check(AgentForwarder *af, const char *fingerprint)
{
    switch (af->policy) {
        case AGENT_FWD_DISABLED:
            return 0;
        case AGENT_FWD_ALLOW:
            af->total_uses++;
            return 1;
        case AGENT_FWD_ALLOW_LISTED:
            for (int i = 0; i < af->num_keys; i++) {
                if (strcmp(af->keys[i].fingerprint, fingerprint) == 0) {
                    if (af->keys[i].allowed) {
                        af->keys[i].use_count++;
                        af->total_uses++;
                        return 1;
                    }
                    return 0;
                }
            }
            return 0;
        case AGENT_FWD_CONFIRM:
        default:
            return -1; /* caller should prompt user */
    }
}

int agentfwd_remove_key(AgentForwarder *af, const char *fingerprint)
{
    for (int i = 0; i < af->num_keys; i++) {
        if (strcmp(af->keys[i].fingerprint, fingerprint) == 0) {
            for (int j = i; j < af->num_keys - 1; j++)
                af->keys[j] = af->keys[j + 1];
            af->num_keys--;
            return 0;
        }
    }
    return -1;
}
