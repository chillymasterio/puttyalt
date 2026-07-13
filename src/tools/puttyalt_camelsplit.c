/* puttyalt_camelsplit.c - Split camelCase into words.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
int cml_split(const char *s,char *out,int outlen){
    if(!s||!out)return -1;
    int o=0;
    for(int i=0;s[i]&&o<outlen-1;i++){ if(i>0&&isupper((unsigned char)s[i])&&!isupper((unsigned char)s[i-1]))out[o++]=' '; if(o<outlen-1)out[o++]=s[i]; }
    out[o]=0; return o;
}
