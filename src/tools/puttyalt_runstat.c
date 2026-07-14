/* puttyalt_runstat.c - Running statistics (Welford).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <math.h>
typedef struct { long n; double mean; double m2; } RunStat;
void rst_init(RunStat *r){ if(r){r->n=0;r->mean=0;r->m2=0;} }
void rst_add(RunStat *r,double x){ if(!r)return; r->n++; double d=x-r->mean; r->mean+=d/r->n; r->m2+=d*(x-r->mean); }
double rst_variance(const RunStat *r){ return (r&&r->n>1)?r->m2/(r->n-1):0; }
double rst_stddev(const RunStat *r){ return sqrt(rst_variance(r)); }
