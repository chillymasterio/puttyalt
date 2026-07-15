/* puttyalt_auditchain2.c - Append-only audit chain with hash links.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
uint32_t adt_link(uint32_t prev,const char *action,const char *user){
    uint32_t h=prev^2166136261u;
    for(const char *p=action;p&&*p;p++){h^=(unsigned char)*p;h*=16777619u;}
    for(const char *p=user;p&&*p;p++){h^=(unsigned char)*p;h*=16777619u;}
    return h;
}
int adt_verify(uint32_t prev,const char *action,const char *user,uint32_t expected){ return adt_link(prev,action,user)==expected; }
