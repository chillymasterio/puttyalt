#include <string.h>
#include <stdio.h>

#define MAX_HOSTS 1024
#define HOST_LEN 256

static char g_known_hosts[MAX_HOSTS][HOST_LEN];
static int g_host_count = 0;

void hostcomplete_add(const char *host)
{
    for (int i = 0; i < g_host_count; i++)
        if (strcmp(g_known_hosts[i], host) == 0) return;
    if (g_host_count >= MAX_HOSTS) return;
    snprintf(g_known_hosts[g_host_count++], HOST_LEN, "%s", host);
}

int hostcomplete_suggest(const char *prefix, char results[][HOST_LEN], int max)
{
    int n = 0, plen = (int)strlen(prefix);
    for (int i = 0; i < g_host_count && n < max; i++)
        if (strncmp(g_known_hosts[i], prefix, plen) == 0)
            snprintf(results[n++], HOST_LEN, "%s", g_known_hosts[i]);
    return n;
}

int hostcomplete_load_known_hosts(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        char *space = strchr(line, ' ');
        if (space) *space = 0;
        char *comma = strchr(line, ',');
        if (comma) *comma = 0;
        line[strcspn(line, "\r\n")] = 0;
        if (line[0]) hostcomplete_add(line);
    }
    fclose(fp);
    return g_host_count;
}

int hostcomplete_count(void) { return g_host_count; }
