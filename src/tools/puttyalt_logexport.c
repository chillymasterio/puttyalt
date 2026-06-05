/* puttyalt_logexport.c - Structured log export (JSON lines / CSV / plain). */
#include <string.h>
#include <stdio.h>
enum lx_fmt { LX_PLAIN=0, LX_JSONL=1, LX_CSV=2 };
int logexport_line(int fmt, const char *ts, const char *level, const char *msg, char *out, int outlen) {
    if(!out) return -1;
    switch (fmt) {
        case LX_JSONL: return snprintf(out,outlen,"{\"ts\":\"%s\",\"level\":\"%s\",\"msg\":\"%s\"}",ts?ts:"",level?level:"",msg?msg:"");
        case LX_CSV:   return snprintf(out,outlen,"%s,%s,\"%s\"",ts?ts:"",level?level:"",msg?msg:"");
        default:       return snprintf(out,outlen,"[%s] %s: %s",ts?ts:"",level?level:"",msg?msg:"");
    }
}
int logexport_header(int fmt, char *out, int outlen) {
    if(!out) return -1;
    if (fmt==LX_CSV) return snprintf(out,outlen,"timestamp,level,message");
    return 0;
}
const char *logexport_extension(int fmt) {
    switch(fmt) { case LX_JSONL: return ".jsonl"; case LX_CSV: return ".csv"; default: return ".log"; }
}
