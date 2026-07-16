/* puttyalt_keyname2.c - Key name lookup and normalization.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
int knm_normalize(const char *name,char *out,int outlen){
    if(!name||!out)return -1;
    int o=0,at_start=1;
    for(;*name&&o<outlen-1;name++){ if(*name=='+'){out[o++]='+';at_start=1;} else { out[o++]=at_start?(char)toupper((unsigned char)*name):(char)tolower((unsigned char)*name); at_start=0; } }
    out[o]=0; return o;
}
int knm_is_modifier(const char *name){ return name&&(!strcasecmp(name,"ctrl")||!strcasecmp(name,"alt")||!strcasecmp(name,"shift")||!strcasecmp(name,"super")); }
