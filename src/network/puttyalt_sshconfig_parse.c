#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_SSH_HOSTS 128
typedef struct { char alias[128]; char hostname[256]; int port; char user[64]; char identity[256]; char proxy[256]; } SshConfigHost;
static SshConfigHost g_hosts[MAX_SSH_HOSTS]; static int g_host_count = 0;
int sshconfig_parse(const char *path) {
    FILE *fp = fopen(path, "r"); if (!fp) return -1;
    char line[1024]; int cur = -1;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        char *p = line; while (*p == ' ' || *p == '\t') p++;
        if (!*p || *p == '#') continue;
        if (strncmp(p, "Host ", 5) == 0 && p[5] != '*') {
            if (g_host_count >= MAX_SSH_HOSTS) break;
            cur = g_host_count++; memset(&g_hosts[cur], 0, sizeof(SshConfigHost));
            snprintf(g_hosts[cur].alias, 128, "%s", p + 5); g_hosts[cur].port = 22;
        } else if (cur >= 0) {
            if (strncmp(p,"HostName ",9)==0) snprintf(g_hosts[cur].hostname,256,"%s",p+9);
            else if (strncmp(p,"Port ",5)==0) g_hosts[cur].port = atoi(p+5);
            else if (strncmp(p,"User ",5)==0) snprintf(g_hosts[cur].user,64,"%s",p+5);
            else if (strncmp(p,"IdentityFile ",13)==0) snprintf(g_hosts[cur].identity,256,"%s",p+13);
            else if (strncmp(p,"ProxyJump ",10)==0) snprintf(g_hosts[cur].proxy,256,"%s",p+10);
        }
    }
    fclose(fp); return g_host_count;
}
int sshconfig_count(void) { return g_host_count; }
SshConfigHost *sshconfig_get(int idx) { return (idx>=0 && idx<g_host_count) ? &g_hosts[idx] : NULL; }
