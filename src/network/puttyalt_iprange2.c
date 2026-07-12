/* puttyalt_iprange2.c - IPv4 range membership by bounds.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
typedef struct { uint32_t lo; uint32_t hi; } IpRange;
int ipr_contains(IpRange r,uint32_t ip){ return ip>=r.lo&&ip<=r.hi; }
long ipr_size(IpRange r){ return (long)r.hi-(long)r.lo+1; }
int ipr_overlaps(IpRange a,IpRange b){ return a.lo<=b.hi&&b.lo<=a.hi; }
