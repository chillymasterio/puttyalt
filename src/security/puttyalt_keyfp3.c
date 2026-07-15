/* puttyalt_keyfp3.c - Format SSH key fingerprints (SHA256/MD5).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
int kfp_md5(const uint8_t *raw,int n,char *out,int outlen){
    if(!raw||!out)return -1;
    int o=0;
    for(int i=0;i<n&&o<outlen-3;i++)o+=snprintf(out+o,outlen-o,"%s%02x",i?":":"",raw[i]);
    return o;
}
uint32_t kfp_shortid(const uint8_t *raw,int n){ uint32_t h=2166136261u; for(int i=0;i<n;i++){h^=raw[i];h*=16777619u;} return h; }
