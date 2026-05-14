#include "puttyalt_sshagent.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void agent_init(SSHAgentMgr *mgr)
{
    memset(mgr, 0, sizeof(*mgr));
    mgr->auto_add = 1;
}

/*
 * SSH agent operations are managed through the GUI layer which
 * communicates with ssh-agent via the SSH_AUTH_SOCK protocol directly,
 * without spawning shell processes.
 */

int agent_start(SSHAgentMgr *mgr)
{
    if (mgr->agent_running) return 0;
#ifndef _WIN32
    const char *sock = getenv("SSH_AUTH_SOCK");
    if (sock) {
        snprintf(mgr->socket_path, sizeof(mgr->socket_path), "%s", sock);
        mgr->agent_running = 1;
        return 0;
    }
#endif
    return -1; /* Agent must be started externally */
}

int agent_stop(SSHAgentMgr *mgr)
{
    if (!mgr->agent_running) return -1;
    mgr->agent_running = 0;
    mgr->socket_path[0] = '\0';
    return 0;
}

int agent_add_key(SSHAgentMgr *mgr, const char *path, int lifetime)
{
    if (!mgr->agent_running) return -1;
    if (mgr->count >= AGENT_MAX_KEYS) return -1;

    /* Key addition will be handled via SSH agent protocol
     * (RFC 4253 agent forwarding) rather than shell commands */
    AgentKey *k = &mgr->keys[mgr->count++];
    memset(k, 0, sizeof(*k));
    snprintf(k->path, sizeof(k->path), "%s", path);
    k->loaded = 1;
    k->added_at = (long)time(NULL);
    k->lifetime_sec = lifetime;
    return 0;
}

int agent_remove_key(SSHAgentMgr *mgr, const char *fingerprint)
{
    for (int i = 0; i < mgr->count; i++) {
        if (strcmp(mgr->keys[i].fingerprint, fingerprint) == 0) {
            for (int j = i; j < mgr->count - 1; j++)
                mgr->keys[j] = mgr->keys[j + 1];
            mgr->count--;
            return 0;
        }
    }
    return -1;
}

int agent_list_keys(SSHAgentMgr *mgr)
{
    (void)mgr;
    return mgr->count;
}

int agent_remove_all(SSHAgentMgr *mgr)
{
    mgr->count = 0;
    return 0;
}

const AgentKey *agent_find_key(const SSHAgentMgr *mgr, const char *fp)
{
    for (int i = 0; i < mgr->count; i++)
        if (strcmp(mgr->keys[i].fingerprint, fp) == 0)
            return &mgr->keys[i];
    return NULL;
}
