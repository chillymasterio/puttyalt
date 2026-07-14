/* puttyalt_shuffle2.c - Deterministic Fisher-Yates shuffle.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
void shf_shuffle(int *a,int n,uint32_t seed){
    if(!a||n<=1)return;
    uint32_t s=seed?seed:1;
    for(int i=n-1;i>0;i--){ s=s*1103515245u+12345u; int j=(int)((s>>16)%(i+1)); int t=a[i];a[i]=a[j];a[j]=t; }
}
