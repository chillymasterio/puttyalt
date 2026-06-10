/* puttyalt_escfilter.c - Escape sequence filter (strip/sanitize control codes). */
#include <string.h>
int escfilter_strip_all(const char *in, char *out, int outlen) {
    if(!in||!out) return -1; int o=0;
    for (const char *p=in; *p && o<outlen-1; ) {
        if (*p=='\x1b') {
            p++;
            if (*p=='[') { p++; while(*p && !((*p>='@'&&*p<='~'))) p++; if(*p)p++; }
            else if (*p==']') { p++; while(*p && *p!='\x07' && *p!='\x1b') p++; if(*p=='\x07')p++; else if(*p=='\x1b'&&p[1]=='\\')p+=2; }
            else if (*p) p++;
        } else if ((unsigned char)*p<32 && *p!='\n' && *p!='\t' && *p!='\r') {
            p++; /* strip other control chars */
        } else out[o++]=*p++;
    }
    out[o]=0; return o;
}
int escfilter_is_safe(const char *in) {
    for (const char *p=in;*p;p++) {
        if (*p=='\x1b') return 0;
        if ((unsigned char)*p<32 && *p!='\n' && *p!='\t' && *p!='\r') return 0;
    }
    return 1;
}
int escfilter_count_sequences(const char *in) {
    int n=0; for(const char *p=in;*p;p++) if(*p=='\x1b') n++; return n;
}
