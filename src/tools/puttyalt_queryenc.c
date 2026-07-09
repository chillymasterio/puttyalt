/* puttyalt_queryenc.c - Encode key-value pairs as query string.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
static int qe_hex(int c){ return c<10?'0'+c:'A'+c-10; }
int qec_pair(const char *key,const char *val,int first,char *out,int outlen){
    if(!key||!out)return -1;
    int o=0;
    if(!first&&o<outlen-1)out[o++]='&';
    for(const char *s=key;*s&&o<outlen-3;s++){ unsigned char c=*s; if(isalnum(c)||c=='-'||c=='_')out[o++]=c; else {out[o++]='%';out[o++]=qe_hex(c>>4);out[o++]=qe_hex(c&15);} }
    if(o<outlen-1)out[o++]='=';
    for(const char *s=val?val:"";*s&&o<outlen-3;s++){ unsigned char c=*s; if(isalnum(c)||c=='-'||c=='_')out[o++]=c; else {out[o++]='%';out[o++]=qe_hex(c>>4);out[o++]=qe_hex(c&15);} }
    out[o]=0; return o;
}
