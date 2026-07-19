/* puttyalt_summarystat.c - Five-number summary of a dataset.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { double min,q1,median,q3,max; } Summary5;
static void sm5_sort(double *a,int n){ for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(a[j]<a[i]){double t=a[i];a[i]=a[j];a[j]=t;} }
int sm5_compute(double *data,int n,Summary5 *out){
    if(!data||!out||n<=0)return -1;
    sm5_sort(data,n);
    out->min=data[0]; out->max=data[n-1]; out->median=data[n/2];
    out->q1=data[n/4]; out->q3=data[3*n/4];
    return 0;
}
