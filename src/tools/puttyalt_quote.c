/* puttyalt_quote.c - Shell quoting/escaping (POSIX + Windows). */
#include <string.h>
int quote_posix(const char *in, char *out, int outlen) {
    if(!in||!out) return -1;
    int needs=0; for(const char*p=in;*p;p++) if(strchr(" \t\n\"'$`\\<>|&;()",*p)){needs=1;break;}
    if (!needs) { int l=(int)strlen(in); if(l<outlen){ memcpy(out,in,l); out[l]=0; return l; } return -1; }
    int o=0; if(o<outlen-1) out[o++]='\'';
    for (const char *p=in;*p && o<outlen-4;p++) {
        if (*p=='\'') { out[o++]='\''; out[o++]='\\'; out[o++]='\''; out[o++]='\''; }
        else out[o++]=*p;
    }
    if(o<outlen-1) out[o++]='\''; out[o]=0; return o;
}
int quote_windows(const char *in, char *out, int outlen) {
    if(!in||!out) return -1;
    int needs=0; for(const char*p=in;*p;p++) if(strchr(" \t\"",*p)){needs=1;break;}
    if (!needs) { int l=(int)strlen(in); if(l<outlen){ memcpy(out,in,l); out[l]=0; return l; } return -1; }
    int o=0; if(o<outlen-1) out[o++]='"';
    for (const char *p=in;*p && o<outlen-2;p++) { if(*p=='"'&&o<outlen-3){out[o++]='\\';} out[o++]=*p; }
    if(o<outlen-1) out[o++]='"'; out[o]=0; return o;
}
