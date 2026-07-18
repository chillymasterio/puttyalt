/* puttyalt_nanoid.c - Generate short URL-safe IDs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
static const char NI_AL[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
int nid_generate(uint64_t seed,int len,char *out,int outlen){
    if(!out||len<=0||len>=outlen)return -1;
    uint64_t s=seed?seed:1;
    for(int i=0;i<len;i++){ s=s*6364136223846793005ULL+1442695040888963407ULL; out[i]=NI_AL[(s>>40)&63]; }
    out[len]=0; return len;
}
