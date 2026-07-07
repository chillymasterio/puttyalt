/* puttyalt_xxlite.c - Lightweight non-cryptographic hash.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
uint32_t xxl_hash(const void *data,int n,uint32_t seed){
    const unsigned char *p=data; uint32_t h=seed+374761393u;
    for(int i=0;i<n;i++){ h+=p[i]*3266489917u; h=(h<<17)|(h>>15); h*=668265263u; }
    h^=h>>15; h*=2246822519u; h^=h>>13; h*=3266489917u; h^=h>>16;
    return h;
}
