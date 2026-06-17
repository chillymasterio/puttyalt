/* puttyalt_retryafter.c - Parse Retry-After header values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stdlib.h>
/* Parse a Retry-After value. If it is delta-seconds, return that.
   Returns -1 for a date form (caller must handle), -2 on error. */
long ra_seconds(const char *s) {
    if (!s) return -2;
    while (*s == ' ') s++;
    int alldigit = 1;
    for (const char *p = s; *p && *p != '\r' && *p != '\n'; p++)
        if (!isdigit((unsigned char)*p)) { alldigit = 0; break; }
    if (alldigit && *s) return atol(s);
    return -1;
}
/* Clamp a retry delay to sane bounds. */
long ra_clamp(long seconds, long min, long max) {
    if (seconds < min) return min;
    if (seconds > max) return max;
    return seconds;
}
