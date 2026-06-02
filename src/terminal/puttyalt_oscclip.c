/* puttyalt_oscclip.c - OSC 52 clipboard set/query sequence parser. */
#include <string.h>
#include <stdio.h>
enum oc_op { OC_NONE=0, OC_SET=1, OC_QUERY=2 };
typedef struct { int op; char selection; char b64[1024]; int b64_len; } OscClip;
int oscclip_parse(const char *seq, int len, OscClip *out) {
    if(!seq||!out||len<5) return -1; memset(out,0,sizeof(*out));
    /* expect: 52;<selection>;<base64-or-?> */
    if (strncmp(seq,"52;",3)!=0) return -1;
    const char *p=seq+3;
    out->selection = (*p && *p!=';') ? *p : 'c';
    const char *semi=strchr(p,';'); if(!semi) return -1;
    p=semi+1;
    if (*p=='?') { out->op=OC_QUERY; return 0; }
    out->op=OC_SET;
    int n=0; while (*p && n<1023 && (p-seq)<len) out->b64[n++]=*p++;
    out->b64[n]=0; out->b64_len=n; return 0;
}
int oscclip_is_clipboard_seq(const char *seq, int len) {
    return (seq && len>=3 && strncmp(seq,"52;",3)==0)?1:0;
}
int oscclip_format_set(char sel, const char *b64, char *out, int outlen) {
    return snprintf(out,outlen,"52;%c;%s",sel,b64?b64:"");
}
