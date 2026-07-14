/* puttyalt_dedupe2.c - Remove adjacent duplicates in sorted array.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int ddp_ints(int *a,int n){ if(n<=1)return n; int w=1; for(int i=1;i<n;i++)if(a[i]!=a[w-1])a[w++]=a[i]; return w; }
int ddp_count_distinct(const int *a,int n){ if(n<=0)return 0; int c=1; for(int i=1;i<n;i++)if(a[i]!=a[i-1])c++; return c; }
