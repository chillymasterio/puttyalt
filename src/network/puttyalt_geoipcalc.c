/* puttyalt_geoipcalc.c - Map IP ranges to region codes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdint.h>
#include <string.h>
#define GI_MAX 32
typedef struct { uint32_t lo, hi; char region[4]; } GiRange;
typedef struct { GiRange r[GI_MAX]; int n; } GeoIpTable;
void gi2_init(GeoIpTable *t) { if (t) t->n = 0; }
int gi2_add(GeoIpTable *t, uint32_t lo, uint32_t hi, const char *region) {
    if (!t || t->n >= GI_MAX || !region) return -1;
    t->r[t->n].lo = lo; t->r[t->n].hi = hi;
    strncpy(t->r[t->n].region, region, 3); t->r[t->n].region[3] = 0;
    t->n++;
    return 0;
}
const char *gi2_lookup(const GeoIpTable *t, uint32_t ip) {
    if (!t) return "??";
    for (int i = 0; i < t->n; i++) if (ip >= t->r[i].lo && ip <= t->r[i].hi) return t->r[i].region;
    return "??";
}
