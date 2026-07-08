/* puttyalt_attrrun.c - Group cells into attribute runs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define AR_MAX 32
typedef struct { int start; int len; unsigned short attr; } ArRun;
/* Split a row of attrs into runs. Returns run count. */
int arn_compute(const unsigned short *attrs,int n,ArRun *out,int cap){
    if(!attrs||!out||n<=0)return 0;
    int rc=0; out[0].start=0; out[0].attr=attrs[0]; out[0].len=1;
    for(int i=1;i<n;i++){ if(attrs[i]==out[rc].attr)out[rc].len++; else { if(++rc>=cap)break; out[rc].start=i; out[rc].attr=attrs[i]; out[rc].len=1; } }
    return rc+1;
}
