/* puttyalt_sdbmhash.c - SDBM string hash.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t sdbm_hash(const char *s){ uint32_t h=0; for(;*s;s++)h=(unsigned char)*s+(h<<6)+(h<<16)-h; return h; }
uint32_t sdbm_hashn(const void *d,int n){ const unsigned char *p=d; uint32_t h=0; for(int i=0;i<n;i++)h=p[i]+(h<<6)+(h<<16)-h; return h; }
