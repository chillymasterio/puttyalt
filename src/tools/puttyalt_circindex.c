/* puttyalt_circindex.c - Circular index arithmetic helpers.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int cix_wrap(int i, int n){ if(n<=0)return 0; i%=n; if(i<0)i+=n; return i; }
int cix_next(int i, int n){ return cix_wrap(i+1,n); }
int cix_prev(int i, int n){ return cix_wrap(i-1,n); }
int cix_distance(int a, int b, int n){ int d=cix_wrap(b-a,n); return d; }
