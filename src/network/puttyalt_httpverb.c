/* puttyalt_httpverb.c - HTTP verb properties table.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
typedef struct { int safe, idempotent, cacheable, body; } VerbProps;
int hvb_props(const char *verb, VerbProps *out){
    if(!verb||!out)return -1;
    struct{const char*v;int s,i,c,b;} t[]={ {"GET",1,1,1,0},{"HEAD",1,1,1,0},{"POST",0,0,0,1},
        {"PUT",0,1,0,1},{"DELETE",0,1,0,0},{"PATCH",0,0,0,1},{"OPTIONS",1,1,0,0} };
    for(unsigned i=0;i<sizeof(t)/sizeof(t[0]);i++) if(!strcmp(verb,t[i].v)){ out->safe=t[i].s; out->idempotent=t[i].i; out->cacheable=t[i].c; out->body=t[i].b; return 0; }
    return -1;
}
