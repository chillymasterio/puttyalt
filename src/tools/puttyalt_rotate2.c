/* puttyalt_rotate2.c - Rotate arrays left/right.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
static void rot_reverse(int *a,int lo,int hi){ while(lo<hi){int t=a[lo];a[lo]=a[hi];a[hi]=t;lo++;hi--;} }
void rot_left(int *a,int n,int k){ if(!a||n<=1)return; k%=n; if(k<0)k+=n; rot_reverse(a,0,k-1); rot_reverse(a,k,n-1); rot_reverse(a,0,n-1); }
void rot_right(int *a,int n,int k){ if(!a||n<=1)return; k%=n; rot_left(a,n,n-k); }
