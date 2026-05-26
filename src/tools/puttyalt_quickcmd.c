#include <string.h>
#include <stdio.h>
#define MAX_QUICK 64
typedef struct { char name[64]; char command[512]; char hotkey[32]; } QuickCmd;
static QuickCmd g_cmds[MAX_QUICK]; static int g_cmd_count = 0;
int quickcmd_add(const char *name, const char *cmd, const char *hotkey) {
    if (g_cmd_count >= MAX_QUICK) return -1;
    QuickCmd *q = &g_cmds[g_cmd_count]; memset(q, 0, sizeof(*q));
    snprintf(q->name, 64, "%s", name); snprintf(q->command, 512, "%s", cmd);
    if (hotkey) snprintf(q->hotkey, 32, "%s", hotkey); return g_cmd_count++;
}
const char *quickcmd_get(const char *name) {
    for (int i = 0; i < g_cmd_count; i++) if (strcmp(g_cmds[i].name, name) == 0) return g_cmds[i].command;
    return NULL;
}
int quickcmd_search(const char *query, char results[][64], int max) {
    int n = 0;
    for (int i = 0; i < g_cmd_count && n < max; i++)
        if (strstr(g_cmds[i].name, query) || strstr(g_cmds[i].command, query))
            snprintf(results[n++], 64, "%s", g_cmds[i].name);
    return n;
}
