/* puttyalt_timeago2.c - Relative time descriptions.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int ta2_describe(long seconds_ago, char *out, int outlen) {
    if (!out) return -1;
    if (seconds_ago < 0) seconds_ago = 0;
    if (seconds_ago < 10) return snprintf(out, outlen, "just now");
    if (seconds_ago < 60) return snprintf(out, outlen, "%ld seconds ago", seconds_ago);
    if (seconds_ago < 3600) { long m = seconds_ago/60; return snprintf(out, outlen, "%ld minute%s ago", m, m==1?"":"s"); }
    if (seconds_ago < 86400) { long h = seconds_ago/3600; return snprintf(out, outlen, "%ld hour%s ago", h, h==1?"":"s"); }
    if (seconds_ago < 2592000) { long d = seconds_ago/86400; return snprintf(out, outlen, "%ld day%s ago", d, d==1?"":"s"); }
    long mo = seconds_ago/2592000;
    return snprintf(out, outlen, "%ld month%s ago", mo, mo==1?"":"s");
}
