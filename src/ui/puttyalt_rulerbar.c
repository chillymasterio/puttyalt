/* puttyalt_rulerbar.c - Column ruler rendering.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int rul_render(int width,int interval,char *out,int outlen){
    if(!out||width<=0||interval<=0)return -1;
    int o=0;
    for(int i=0;i<width&&o<outlen-1;i++){ if(i%interval==0)out[o++]='|'; else if(i%interval==interval/2)out[o++]='+'; else out[o++]='-'; }
    out[o]=0; return o;
}
