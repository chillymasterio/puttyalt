/* puttyalt_tabletext.c - Render a two-column key/value table.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
int tbt_row(const char *key,const char *val,int keywidth,char *out,int outlen){
    if(!key||!val||!out)return -1;
    return snprintf(out,outlen,"%-*s : %s",keywidth,key,val);
}
