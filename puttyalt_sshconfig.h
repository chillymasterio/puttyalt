#ifndef PUTTYALT_SSHCONFIG_H
#define PUTTYALT_SSHCONFIG_H

#define SSHCFG_MAX_HOSTS 128

typedef struct {
    char alias[128];
    char hostname[256];
    char user[128];
    int port;
    char identity_file[512];
    char proxy_command[512];
    int forward_agent;
    int compression;
    char local_forward[256];
    char remote_forward[256];
    char jump_host[128];
    int server_alive_interval;
    int connection_timeout;
} SSHHostConfig;

typedef struct {
    SSHHostConfig hosts[SSHCFG_MAX_HOSTS];
    int count;
    char config_path[512];
} SSHConfigParser;

int  sshconfig_parse(SSHConfigParser *p, const char *path);
int  sshconfig_write(const SSHConfigParser *p, const char *path);
const SSHHostConfig *sshconfig_find(const SSHConfigParser *p, const char *alias);
int  sshconfig_add(SSHConfigParser *p, const SSHHostConfig *host);
int  sshconfig_remove(SSHConfigParser *p, const char *alias);

#endif
