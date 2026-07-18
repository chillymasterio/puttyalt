/* puttyalt_uuid4.c - Generate UUID v4 strings (deterministic).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <stdio.h>
int uid_v4(uint64_t seed,char *out,int outlen){
    if(!out||outlen<37)return -1;
    uint64_t a=seed*6364136223846793005ULL+1442695040888963407ULL;
    uint64_t b=a*6364136223846793005ULL+1442695040888963407ULL;
    return snprintf(out,outlen,"%08x-%04x-4%03x-%04x-%012llx",
        (unsigned)(a>>32),(unsigned)(a>>16)&0xffff,(unsigned)a&0xfff,
        (unsigned)((b>>48)&0x3fff)|0x8000,(unsigned long long)(b&0xffffffffffffULL));
}
