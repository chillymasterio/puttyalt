/* puttyalt_cmdrunner.c - Batch command runner across hosts with result aggregation. */
#include <string.h>
#include <stdio.h>
#define CR_MAX 32
#define CR_HOST 64
#define CR_OUT 160
typedef struct { char host[CR_HOST]; int exit_code; char output[CR_OUT]; int done; } cr_result;
typedef struct { cr_result r[CR_MAX]; int n; char command[256]; } CmdRunner;
void cmdrunner_init(CmdRunner *c, const char *command) {
    if(!c) return; memset(c,0,sizeof(*c)); snprintf(c->command,256,"%s",command?command:"");
}
int cmdrunner_add_host(CmdRunner *c, const char *host) {
    if(!c||c->n>=CR_MAX||!host) return -1; snprintf(c->r[c->n].host,CR_HOST,"%s",host); c->r[c->n].exit_code=-1; return c->n++;
}
int cmdrunner_record(CmdRunner *c, int idx, int exit_code, const char *output) {
    if(!c||idx<0||idx>=c->n) return -1;
    c->r[idx].exit_code=exit_code; snprintf(c->r[idx].output,CR_OUT,"%s",output?output:""); c->r[idx].done=1; return 0;
}
int cmdrunner_success_count(const CmdRunner *c) {
    if(!c) return -1; int n=0; for(int i=0;i<c->n;i++) if(c->r[i].done && c->r[i].exit_code==0)n++; return n;
}
int cmdrunner_summary(const CmdRunner *c, char *buf, int buflen) {
    if(!c||!buf) return -1; int pos=0;
    pos+=snprintf(buf+pos,buflen-pos,"cmd: %s\n",c->command);
    for (int i=0;i<c->n && pos<buflen;i++)
        pos+=snprintf(buf+pos,buflen-pos,"  %s: %s (rc=%d)\n",c->r[i].host,c->r[i].done?(c->r[i].exit_code==0?"OK":"FAIL"):"pending",c->r[i].exit_code);
    return pos;
}
int cmdrunner_all_done(const CmdRunner *c) { if(!c) return -1; for(int i=0;i<c->n;i++) if(!c->r[i].done) return 0; return 1; }
