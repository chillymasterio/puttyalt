/* puttyalt_cmdparse2.c - Split a command line respecting quotes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int cmp_split(const char *line, char *out, int outlen, int *offsets, int maxarg){
    if(!line||!out||!offsets)return -1;
    int o=0,n=0,inq=0; char q=0;
    const char *p=line;
    while(*p){
        while(*p==' '||*p=='\t')p++;
        if(!*p)break;
        if(n>=maxarg)return -1;
        offsets[n++]=o;
        while(*p&&(inq||(*p!=' '&&*p!='\t'))){
            if(!inq&&(*p=='"'||*p=='\'')){inq=1;q=*p;p++;continue;}
            if(inq&&*p==q){inq=0;p++;continue;}
            if(o<outlen-1)out[o++]=*p;
            p++;
        }
        if(o<outlen-1)out[o++]=0;
    }
    return n;
}
