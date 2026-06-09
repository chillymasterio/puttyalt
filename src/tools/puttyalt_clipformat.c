/* puttyalt_clipformat.c - Clipboard formatting (strip ANSI, normalize line endings). */
#include <string.h>
int clipformat_strip_ansi(const char *in, char *out, int outlen) {
    if(!in||!out) return -1; int o=0;
    for (const char *p=in; *p && o<outlen-1; ) {
        if (*p=='\x1b' && p[1]=='[') {
            p+=2; while (*p && !((*p>='@'&&*p<='~'))) p++; if(*p) p++;
        } else out[o++]=*p++;
    }
    out[o]=0; return o;
}
int clipformat_normalize_eol(const char *in, char *out, int outlen, int to_crlf) {
    if(!in||!out) return -1; int o=0;
    for (const char *p=in; *p && o<outlen-2; p++) {
        if (*p=='\r') { if (p[1]=='\n') continue; if(to_crlf){out[o++]='\r';} out[o++]='\n'; }
        else if (*p=='\n') { if(to_crlf) out[o++]='\r'; out[o++]='\n'; }
        else out[o++]=*p;
    }
    out[o]=0; return o;
}
int clipformat_trim_trailing(const char *in, char *out, int outlen) {
    if(!in||!out) return -1;
    int len=(int)strlen(in); if(len>=outlen)len=outlen-1;
    memcpy(out,in,len); out[len]=0;
    /* trim trailing whitespace per line */
    int o=0;
    for (int i=0;i<len;i++) {
        if (out[i]=='\n') { while(o>0&&(out[o-1]==' '||out[o-1]=='\t'))o--; out[o++]='\n'; }
        else out[o++]=out[i];
    }
    while(o>0&&(out[o-1]==' '||out[o-1]=='\t'||out[o-1]=='\n'))o--;
    out[o]=0; return o;
}
