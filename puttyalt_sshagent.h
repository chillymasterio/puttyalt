#ifndef PUTTYALT_SSHAGENT_H
#define PUTTYALT_SSHAGENT_H

#define AGENT_MAX_KEYS 32

typedef enum {
    AGENT_KEY_RSA,
    AGENT_KEY_ED25519,
    AGENT_KEY_ECDSA,
    AGENT_KEY_DSA
} AgentKeyType;

typedef struct {
    char fingerprint[128];
    char comment[256];
    char path[512];
    AgentKeyType type;
    int bits;
    int loaded;
    long added_at;
    int lifetime_sec;
    int confirm_use;
} AgentKey;

typedef struct {
    AgentKey keys[AGENT_MAX_KEYS];
    int count;
    int agent_running;
    char socket_path[256];
    int auto_add;
} SSHAgentMgr;

void agent_init(SSHAgentMgr *mgr);
int  agent_start(SSHAgentMgr *mgr);
int  agent_stop(SSHAgentMgr *mgr);
int  agent_add_key(SSHAgentMgr *mgr, const char *path, int lifetime);
int  agent_remove_key(SSHAgentMgr *mgr, const char *fingerprint);
int  agent_list_keys(SSHAgentMgr *mgr);
int  agent_remove_all(SSHAgentMgr *mgr);
const AgentKey *agent_find_key(const SSHAgentMgr *mgr, const char *fingerprint);

#endif
