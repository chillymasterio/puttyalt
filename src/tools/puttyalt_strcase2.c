/* puttyalt_strcase2.c - snake_case / camelCase / kebab conversions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
int scs_to_camel(const char *s, char *out, int outlen){
    if(!s||!out)return -1;
    int o=0,up=0;
    for(;*s&&o<outlen-1;s++){ if(*s=='_'||*s=='-'||*s==' '){up=1;} else { out[o++]=up?(char)toupper((unsigned char)*s):*s; up=0; } }
    out[o]=0; return o;
}
int scs_to_kebab(const char *s, char *out, int outlen){
    if(!s||!out)return -1;
    int o=0;
    for(;*s&&o<outlen-1;s++){ if(isupper((unsigned char)*s)){ if(o>0&&o<outlen-2)out[o++]='-'; out[o++]=(char)tolower((unsigned char)*s); } else if(*s=='_'||*s==' ')out[o++]='-'; else out[o++]=*s; }
    out[o]=0; return o;
}
