/* puttyalt_confhash.c - Hash config content for change detection.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t cfh_hash(const char *content,int n){ uint32_t h=2166136261u; for(int i=0;i<n;i++){h^=(unsigned char)content[i];h*=16777619u;} return h; }
int cfh_changed(uint32_t old_hash,const char *content,int n){ return cfh_hash(content,n)!=old_hash; }
