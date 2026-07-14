/* puttyalt_binsearch2.c - Binary search variants.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
int bsr_find(const int *a,int n,int key){ int lo=0,hi=n-1; while(lo<=hi){ int m=(lo+hi)/2; if(a[m]==key)return m; if(a[m]<key)lo=m+1; else hi=m-1; } return -1; }
int bsr_lower(const int *a,int n,int key){ int lo=0,hi=n; while(lo<hi){ int m=(lo+hi)/2; if(a[m]<key)lo=m+1; else hi=m; } return lo; }
int bsr_upper(const int *a,int n,int key){ int lo=0,hi=n; while(lo<hi){ int m=(lo+hi)/2; if(a[m]<=key)lo=m+1; else hi=m; } return lo; }
