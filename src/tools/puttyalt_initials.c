/* puttyalt_initials.c - Extract initials from a name.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
int ini_extract(const char *name,char *out,int outlen){
    if(!name||!out)return -1;
    int o=0,at_start=1;
    for(;*name&&o<outlen-1;name++){ if(isspace((unsigned char)*name)){at_start=1;} else { if(at_start){out[o++]=(char)toupper((unsigned char)*name);at_start=0;} } }
    out[o]=0; return o;
}
