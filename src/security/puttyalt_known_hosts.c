#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_KNOWN 512
typedef struct { char host[256]; int port; char keytype[32]; char fingerprint[128]; int trusted; } KnownHostEntry;
static KnownHostEntry g_kh[MAX_KNOWN]; static int g_kh_count = 0;
int knownhosts_load(const char *path) {
    FILE *fp = fopen(path, "r"); if (!fp) return -1;
    char line[1024]; g_kh_count = 0;
    while (fgets(line, sizeof(line), fp) && g_kh_count < MAX_KNOWN) {
        if (line[0]=='#'||line[0]=='\n') continue;
        KnownHostEntry *k = &g_kh[g_kh_count]; memset(k, 0, sizeof(*k)); k->port = 22; k->trusted = 1;
        char host_part[256];
        if (sscanf(line, "%255s %31s %127s", host_part, k->keytype, k->fingerprint) >= 2) {
            char *bracket = strchr(host_part, '[');
            if (bracket) { char *cb=strchr(bracket,']'); if(cb) { *cb=0; snprintf(k->host,256,"%s",bracket+1); char *cp=strchr(cb+1,':'); if(cp) k->port=atoi(cp+1); } }
            else snprintf(k->host, 256, "%s", host_part);
            g_kh_count++;
        }
    }
    fclose(fp); return g_kh_count;
}
int knownhosts_check(const char *host, int port, const char *fp) {
    for (int i=0;i<g_kh_count;i++) if (strcmp(g_kh[i].host,host)==0 && g_kh[i].port==port)
        return strcmp(g_kh[i].fingerprint,fp)==0 ? 1 : -1;
    return 0;
}
int knownhosts_count(void) { return g_kh_count; }
