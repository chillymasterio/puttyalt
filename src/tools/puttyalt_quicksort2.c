/* puttyalt_quicksort2.c - In-place quicksort of ints.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
static void qs_swap(int *a,int *b){ int t=*a;*a=*b;*b=t; }
static void qs_rec(int *a,int lo,int hi){ if(lo>=hi)return; int pivot=a[(lo+hi)/2],i=lo,j=hi; while(i<=j){ while(a[i]<pivot)i++; while(a[j]>pivot)j--; if(i<=j){qs_swap(&a[i],&a[j]);i++;j--;} } qs_rec(a,lo,j); qs_rec(a,i,hi); }
void qsr_sort(int *a,int n){ if(a&&n>1)qs_rec(a,0,n-1); }
