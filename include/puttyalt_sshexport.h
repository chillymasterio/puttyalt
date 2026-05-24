#ifndef PUTTYALT_SSHEXPORT_H
#define PUTTYALT_SSHEXPORT_H

typedef struct {
    char host_alias[64];
    char hostname[256];
    int  port;
    char user[64];
    char identity_file[512];
    char proxy_jump[256];
    int  forward_agent;
    int  forward_x11;
    int  compression;
    int  keepalive;
    char extra_options[1024];
} SSHConfigEntry;

int sshexport_single(SSHConfigEntry *entry, char *buf, int buflen);
int sshexport_all(SSHConfigEntry *entries, int count, const char *path);
int sshexport_import_line(const char *line, SSHConfigEntry *entry);

#endif
