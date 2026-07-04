/* puttyalt_varinterp.c - Interpolate ${var} with defaults.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef const char *(*VarGet)(const char *name);
int vip_expand(const char *s, VarGet get, char *out, int outlen){
    if(!s||!out)return -1;
    int o=0;
    while(*s&&o<outlen-1){
        if(*s=='$'&&s[1]=='{'){
            const char *end=strchr(s+2,'}');
            if(end){ char name[48]; const char *colon=memchr(s+2,':',end-(s+2));
                int nl=(int)((colon?colon:end)-(s+2)); if(nl>=48)nl=47; memcpy(name,s+2,nl); name[nl]=0;
                const char *val=get?get(name):0;
                if(!val&&colon&&colon[1]=='-')val=colon+2>end?"":0;
                const char *dflt = (colon&&colon[1]=='-')?colon+2:0;
                if(val){ for(;*val&&o<outlen-1;val++)out[o++]=*val; }
                else if(dflt){ for(const char *dp=dflt;dp<end&&o<outlen-1;dp++)out[o++]=*dp; }
                s=end+1; continue;
            }
        }
        out[o++]=*s++;
    }
    out[o]=0; return o;
}
