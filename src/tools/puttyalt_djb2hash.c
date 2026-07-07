/* puttyalt_djb2hash.c - DJB2 string hash.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t djb2_hash(const char *s){ uint32_t h=5381; for(;*s;s++)h=((h<<5)+h)+(unsigned char)*s; return h; }
uint32_t djb2_hashn(const void *d,int n){ const unsigned char *p=d; uint32_t h=5381; for(int i=0;i<n;i++)h=((h<<5)+h)+p[i]; return h; }
