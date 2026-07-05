/* puttyalt_dcsparse.c - Parse DCS (device control string) prefixes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { int params[8]; int nparams; char intermediate; char final; } DcsSeq;
int dcs_parse(const char *body, DcsSeq *out){
    if(!body||!out)return -1;
    out->nparams=0; out->intermediate=0; out->final=0;
    int cur=0,have=0;
    const char *p=body;
    for(;*p;p++){
        if(*p>='0'&&*p<='9'){cur=cur*10+(*p-'0');have=1;}
        else if(*p==';'){ if(out->nparams<8)out->params[out->nparams++]=have?cur:0; cur=0;have=0; }
        else if(*p>=0x20&&*p<=0x2f){ out->intermediate=*p; }
        else { if(have&&out->nparams<8)out->params[out->nparams++]=cur; out->final=*p; return 0; }
    }
    return -1;
}
