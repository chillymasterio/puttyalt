/* puttyalt_keyseqenc.c - Encode special keys to escape sequences.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
int kys_encode(const char *keyname,int app_mode,char *out,int outlen){
    if(!keyname||!out)return -1;
    if(!strcmp(keyname,"Up"))return snprintf(out,outlen,app_mode?"\x1bOA":"\x1b[A");
    if(!strcmp(keyname,"Down"))return snprintf(out,outlen,app_mode?"\x1bOB":"\x1b[B");
    if(!strcmp(keyname,"Right"))return snprintf(out,outlen,app_mode?"\x1bOC":"\x1b[C");
    if(!strcmp(keyname,"Left"))return snprintf(out,outlen,app_mode?"\x1bOD":"\x1b[D");
    if(!strcmp(keyname,"Home"))return snprintf(out,outlen,"\x1b[H");
    if(!strcmp(keyname,"End"))return snprintf(out,outlen,"\x1b[F");
    return -1;
}
