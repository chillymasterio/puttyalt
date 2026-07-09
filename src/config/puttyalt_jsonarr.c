/* puttyalt_jsonarr.c - Iterate top-level JSON array elements.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Find element `idx` start offset in a flat JSON array. Returns offset or -1. */
int jar_element(const char *s,int idx,int *len){
    if(!s||*s!='[')return -1;
    s++;
    int depth=0,elem=0;
    const char *start=s;
    int instr=0;
    for(const char *p=s;*p;p++){
        if(instr){ if(*p=='\\')p++; else if(*p=='"')instr=0; continue; }
        if(*p=='"')instr=1;
        else if(*p=='['||*p=='{')depth++;
        else if(*p==']'||*p=='}'){ if(depth==0){ if(elem==idx){if(len)*len=(int)(p-start);return (int)(start-s)+1;} return -1;} depth--; }
        else if(*p==','&&depth==0){ if(elem==idx){if(len)*len=(int)(p-start);return (int)(start-s)+1;} elem++; start=p+1; }
    }
    return -1;
}
