/* puttyalt_unitparse.c - Parse human sizes/durations (10K, 2h).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Parse "10K","2M","1G" into bytes. Returns -1 on error. */
long up2_size(const char *s) {
    if (!s) return -1;
    long v = 0; int seen = 0;
    while (*s >= '0' && *s <= '9') { v = v*10 + (*s-'0'); s++; seen = 1; }
    if (!seen) return -1;
    switch (*s) {
        case 'k': case 'K': return v * 1024;
        case 'm': case 'M': return v * 1024 * 1024;
        case 'g': case 'G': return v * 1024L * 1024 * 1024;
        case 0: return v;
        default: return -1;
    }
}
/* Parse "30s","5m","2h","1d" into seconds. */
long up2_duration(const char *s) {
    if (!s) return -1;
    long v = 0; int seen = 0;
    while (*s >= '0' && *s <= '9') { v = v*10 + (*s-'0'); s++; seen = 1; }
    if (!seen) return -1;
    switch (*s) {
        case 's': case 0: return v;
        case 'm': return v * 60;
        case 'h': return v * 3600;
        case 'd': return v * 86400;
        default: return -1;
    }
}
