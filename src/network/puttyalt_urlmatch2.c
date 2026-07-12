/* puttyalt_urlmatch2.c - Match URLs against wildcard patterns.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static int um_glob(const char *pat,const char *s){ const char *star=0,*ss=0; while(*s){ if(*pat=='*'){star=pat++;ss=s;} else if(*pat==*s){pat++;s++;} else if(star){pat=star+1;s=++ss;} else return 0; } while(*pat=='*')pat++; return *pat==0; }
int urm_match(const char *pattern,const char *url){ return pattern&&url&&um_glob(pattern,url); }
int urm_match_any(const char *const *patterns,int n,const char *url){ for(int i=0;i<n;i++)if(urm_match(patterns[i],url))return 1; return 0; }
