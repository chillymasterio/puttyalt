/* puttyalt_globlist.c - Match paths against glob include/exclude lists.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static int glb_one(const char *pat, const char *s){
    const char *star=0,*ss=0;
    while(*s){ if(*pat=='?'||*pat==*s){pat++;s++;} else if(*pat=='*'){star=pat++;ss=s;} else if(star){pat=star+1;s=++ss;} else return 0; }
    while(*pat=='*')pat++;
    return *pat==0;
}
/* Return 1 if included: matches an include and no exclude. */
int glbl_match(const char *const *includes, int ninc, const char *const *excludes, int nexc, const char *path){
    int inc = (ninc==0);
    for(int i=0;i<ninc;i++) if(glb_one(includes[i],path)){inc=1;break;}
    if(!inc)return 0;
    for(int i=0;i<nexc;i++) if(glb_one(excludes[i],path))return 0;
    return 1;
}
