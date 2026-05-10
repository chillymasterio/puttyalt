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

#ifndef _WIN32
#include <unistd.h>

int agent_start(SSHAgentMgr *mgr)
{
    if (mgr->agent_running) return 0;
    char *sock = getenv("SSH_AUTH_SOCK");
    if (sock) {
        snprintf(mgr->socket_path, sizeof(mgr->socket_path), "%s", sock);
        mgr->agent_running = 1;
        return 0;
    }
    int ret = system("eval $(ssh-agent -s) > /dev/null 2>&1");
    if (ret == 0) {
        sock = getenv("SSH_AUTH_SOCK");
        if (sock) snprintf(mgr->socket_path, sizeof(mgr->socket_path), "%s", sock);
        mgr->agent_running = 1;
    }
    return ret;
}

int agent_stop(SSHAgentMgr *mgr)
{
    if (!mgr->agent_running) return -1;
    system("ssh-agent -k > /dev/null 2>&1");
    mgr->agent_running = 0;
    return 0;
}

int agent_add_key(SSHAgentMgr *mgr, const char *path, int lifetime)
{
    if (!mgr->agent_running) return -1;
    if (mgr->count >= AGENT_MAX_KEYS) return -1;

    char cmd[1024];
    if (lifetime > 0)
        snprintf(cmd, sizeof(cmd), "ssh-add -t %d '%s' 2>&1", lifetime, path);
    else
        snprintf(cmd, sizeof(cmd), "ssh-add '%s' 2>&1", path);

    int ret = system(cmd);
    if (ret == 0) {
        AgentKey *k = &mgr->keys[mgr->count++];
        memset(k, 0, sizeof(*k));
        snprintf(k->path, sizeof(k->path), "%s", path);
        k->loaded = 1;
        k->added_at = (long)time(NULL);
        k->lifetime_sec = lifetime;
    }
    return ret;
}

int agent_remove_key(SSHAgentMgr *mgr, const char *fingerprint)
{
    for (int i = 0; i < mgr->count; i++) {
        if (strcmp(mgr->keys[i].fingerprint, fingerprint) == 0) {
            char cmd[1024];
            snprintf(cmd, sizeof(cmd), "ssh-add -d '%s' 2>&1", mgr->keys[i].path);
            system(cmd);
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
    return system("ssh-add -l");
}

int agent_remove_all(SSHAgentMgr *mgr)
{
    int ret = system("ssh-add -D 2>&1");
    if (ret == 0) mgr->count = 0;
    return ret;
}

#else
int agent_start(SSHAgentMgr *mgr) { (void)mgr; return -1; }
int agent_stop(SSHAgentMgr *mgr) { (void)mgr; return -1; }
int agent_add_key(SSHAgentMgr *mgr, const char *p, int l) { (void)mgr; (void)p; (void)l; return -1; }
int agent_remove_key(SSHAgentMgr *mgr, const char *f) { (void)mgr; (void)f; return -1; }
int agent_list_keys(SSHAgentMgr *mgr) { (void)mgr; return -1; }
int agent_remove_all(SSHAgentMgr *mgr) { (void)mgr; return -1; }
#endif

const AgentKey *agent_find_key(const SSHAgentMgr *mgr, const char *fp)
{
    for (int i = 0; i < mgr->count; i++)
        if (strcmp(mgr->keys[i].fingerprint, fp) == 0)
            return &mgr->keys[i];
    return NULL;
}
