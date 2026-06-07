/* puttyalt_cmdhelp.c - Inline command help/manpage hint lookup. */
#include <string.h>
#include <stdio.h>
#define CH_MAX 64
#define CH_NAME 24
#define CH_DESC 160
typedef struct { char cmd[CH_NAME]; char synopsis[CH_DESC]; } ch_entry;
typedef struct { ch_entry e[CH_MAX]; int n; } CmdHelp;
void cmdhelp_init(CmdHelp *c) { if(c) memset(c,0,sizeof(*c)); }
int cmdhelp_register(CmdHelp *c, const char *cmd, const char *synopsis) {
    if(!c||c->n>=CH_MAX||!cmd) return -1;
    snprintf(c->e[c->n].cmd,CH_NAME,"%s",cmd); snprintf(c->e[c->n].synopsis,CH_DESC,"%s",synopsis?synopsis:""); c->n++;
    return 0;
}
const char *cmdhelp_lookup(const CmdHelp *c, const char *cmd) {
    if(!c||!cmd) return 0;
    /* match first word only */
    char first[CH_NAME]; int i=0;
    while (cmd[i] && cmd[i]!=' ' && i<CH_NAME-1) { first[i]=cmd[i]; i++; } first[i]=0;
    for (int j=0;j<c->n;j++) if (strcmp(c->e[j].cmd,first)==0) return c->e[j].synopsis;
    return 0;
}
int cmdhelp_count(const CmdHelp *c) { return c?c->n:-1; }
