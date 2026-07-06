/* puttyalt_clockdisp.c - Format clock displays 12/24h.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int clk_24h(int h,int m,int s,char *out,int outlen){ return snprintf(out,outlen,"%02d:%02d:%02d",h%24,m%60,s%60); }
int clk_12h(int h,int m,char *out,int outlen){ const char *ap=h<12?"AM":"PM"; int h12=h%12; if(h12==0)h12=12; return snprintf(out,outlen,"%d:%02d %s",h12,m%60,ap); }
