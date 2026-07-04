/* puttyalt_strstrip.c - Strip characters from string ends.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int sstp_strip(const char *s, const char *chars, char *out, int outlen){
    if(!s||!chars||!out)return -1;
    while(*s && strchr(chars,*s))s++;
    int n=(int)strlen(s);
    while(n>0 && strchr(chars,s[n-1]))n--;
    if(n>=outlen)n=outlen-1;
    memcpy(out,s,n); out[n]=0; return n;
}
int sstp_lstrip(const char *s, const char *chars, char *out, int outlen){
    if(!s||!chars||!out)return -1;
    while(*s && strchr(chars,*s))s++;
    strncpy(out,s,outlen-1); out[outlen-1]=0; return (int)strlen(out);
}
