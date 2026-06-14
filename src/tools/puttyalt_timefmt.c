/* puttyalt_timefmt.c - Format durations and timestamps.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int tfmt_hms(long seconds, char *out, int outlen) {
    if (!out) return -1;
    if (seconds < 0) seconds = 0;
    long h = seconds / 3600, m = (seconds % 3600) / 60, s = seconds % 60;
    return snprintf(out, outlen, "%02ld:%02ld:%02ld", h, m, s);
}
int tfmt_clock(int hour, int min, int sec, char *out, int outlen) {
    return snprintf(out, outlen, "%02d:%02d:%02d", hour % 24, min % 60, sec % 60);
}
int tfmt_compact(long seconds, char *out, int outlen) {
    if (seconds < 60) return snprintf(out, outlen, "%lds", seconds);
    if (seconds < 3600) return snprintf(out, outlen, "%ldm%lds", seconds/60, seconds%60);
    return snprintf(out, outlen, "%ldh%ldm", seconds/3600, (seconds%3600)/60);
}
