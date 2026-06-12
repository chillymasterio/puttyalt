/* puttyalt_oscprogress.c - OSC 9;4 (ConEmu/Windows Terminal) progress indicator parser. */
#include <string.h>
#include <stdio.h>
enum op_state { OP_NONE=0, OP_NORMAL=1, OP_ERROR=2, OP_INDETERM=3, OP_WARN=4 };
typedef struct { int state; int percent; } OscProgress;
int oscprogress_parse(const char *seq, OscProgress *out) {
    if(!seq||!out) return -1;
    memset(out,0,sizeof(*out));
    if (strncmp(seq,"9;4;",4)!=0) return -1;
    const char *p=seq+4; int st=*p-'0'; p++;
    if (st<0||st>4) return -1;
    out->state=st;
    if (*p==';') { p++; int v=0; while(*p>='0'&&*p<='9') v=v*10+(*p++-'0'); out->percent=v>100?100:v; }
    return 0;
}
int oscprogress_is_seq(const char *seq) { return (seq && strncmp(seq,"9;4;",4)==0)?1:0; }
int oscprogress_format(int state, int percent, char *buf, int buflen) {
    return snprintf(buf,buflen,"9;4;%d;%d",state,percent<0?0:(percent>100?100:percent));
}
