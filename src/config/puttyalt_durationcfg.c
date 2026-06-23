/* puttyalt_durationcfg.c - Parse extended duration config values.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Parse compound durations like "1h30m" or "2d4h" into seconds. */
long dc3_parse(const char *s) {
    if (!s) return -1;
    long total = 0, num = 0; int any = 0;
    while (*s) {
        if (isdigit((unsigned char)*s)) { num = num * 10 + (*s - '0'); any = 1; }
        else {
            long mult = 0;
            switch (*s) {
                case 'd': mult = 86400; break;
                case 'h': mult = 3600; break;
                case 'm': mult = 60; break;
                case 's': mult = 1; break;
                default: return -1;
            }
            total += num * mult; num = 0;
        }
        s++;
    }
    if (num > 0) total += num; /* trailing bare number = seconds */
    return any ? total : -1;
}
