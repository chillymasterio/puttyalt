/* puttyalt_strpad.c - Pad and center strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int spd_left(const char *s, char pad, int width, char *out, int outlen){
    if(!s||!out)return -1;
    int n=(int)strlen(s),o=0;
    for(int i=0;i<n&&o<outlen-1;i++)out[o++]=s[i];
    while(o<width&&o<outlen-1)out[o++]=pad;
    out[o]=0; return o;
}
int spd_right(const char *s, char pad, int width, char *out, int outlen){
    if(!s||!out)return -1;
    int n=(int)strlen(s),o=0,p=width-n;
    while(o<p&&o<outlen-1)out[o++]=pad;
    for(int i=0;i<n&&o<outlen-1;i++)out[o++]=s[i];
    out[o]=0; return o;
}
