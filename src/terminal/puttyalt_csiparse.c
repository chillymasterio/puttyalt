/* puttyalt_csiparse.c - CSI parameter parser (numeric params + intermediate + final). */
#include <string.h>
#define CSI_MAX_PARAMS 16
typedef struct { int params[CSI_MAX_PARAMS]; int nparams; char intermediate; char final; int private_marker; } CsiParse;
int csiparse(const char *seq, int len, CsiParse *out) {
    if(!seq||!out||len<1) return -1;
    memset(out,0,sizeof(*out));
    int i=0;
    if (seq[0]=='?'||seq[0]=='<'||seq[0]=='='||seq[0]=='>') { out->private_marker=seq[0]; i=1; }
    int cur=0, have=0;
    for (; i<len; i++) {
        char c=seq[i];
        if (c>='0'&&c<='9') { cur=cur*10+(c-'0'); have=1; }
        else if (c==';') { if(out->nparams<CSI_MAX_PARAMS) out->params[out->nparams++]=cur; cur=0; have=0; }
        else if (c>=0x20 && c<=0x2f) { out->intermediate=c; }
        else if (c>=0x40 && c<=0x7e) {
            if (have||out->nparams>0) { if(out->nparams<CSI_MAX_PARAMS) out->params[out->nparams++]=cur; }
            out->final=c; return 0;
        }
    }
    return -1; /* incomplete */
}
int csiparse_param(const CsiParse *c, int idx, int fallback) {
    if(!c||idx<0||idx>=c->nparams) return fallback;
    return c->params[idx];
}
