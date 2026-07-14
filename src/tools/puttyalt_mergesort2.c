/* puttyalt_mergesort2.c - Bottom-up merge sort of ints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define MS_MAX 512
void mgs_sort(int *a,int n){
    if(!a||n<=1||n>MS_MAX)return;
    int tmp[MS_MAX];
    for(int w=1;w<n;w*=2){ for(int i=0;i<n;i+=2*w){ int l=i,m=i+w<n?i+w:n,r=i+2*w<n?i+2*w:n,li=l,ri=m,ti=l; while(li<m&&ri<r)tmp[ti++]=a[li]<=a[ri]?a[li++]:a[ri++]; while(li<m)tmp[ti++]=a[li++]; while(ri<r)tmp[ti++]=a[ri++]; } memcpy(a,tmp,n*sizeof(int)); }
}
