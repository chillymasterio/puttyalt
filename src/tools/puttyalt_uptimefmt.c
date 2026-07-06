/* puttyalt_uptimefmt.c - Format uptime durations compactly.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int upf_format(long seconds,char *out,int outlen){
    if(!out)return -1;
    long d=seconds/86400,h=seconds%86400/3600,m=seconds%3600/60;
    if(d>0)return snprintf(out,outlen,"%ldd %ldh %ldm",d,h,m);
    if(h>0)return snprintf(out,outlen,"%ldh %ldm",h,m);
    return snprintf(out,outlen,"%ldm",m);
}
