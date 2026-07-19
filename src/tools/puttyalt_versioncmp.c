/* puttyalt_versioncmp.c - Compare dotted version strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdlib.h>
int vcm_compare(const char *a,const char *b){
    if(!a||!b)return 0;
    while(*a||*b){ long va=strtol(a,(char**)&a,10),vb=strtol(b,(char**)&b,10); if(va!=vb)return va<vb?-1:1; if(*a=='.')a++; if(*b=='.')b++; }
    return 0;
}
int vcm_satisfies(const char *version,const char *minimum){ return vcm_compare(version,minimum)>=0; }
