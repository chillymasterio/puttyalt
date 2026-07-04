/* puttyalt_inputsan.c - Sanitize input by removing control bytes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int isan_strip_controls(const char *s, char *out, int outlen){
    if(!s||!out)return -1;
    int o=0;
    for(;*s&&o<outlen-1;s++){ unsigned char c=(unsigned char)*s; if(c>=32||c=='\t'||c=='\n')out[o++]=(char)c; }
    out[o]=0; return o;
}
int isan_has_controls(const char *s){
    for(;*s;s++){ unsigned char c=(unsigned char)*s; if(c<32&&c!='\t'&&c!='\n'&&c!='\r')return 1; }
    return 0;
}
