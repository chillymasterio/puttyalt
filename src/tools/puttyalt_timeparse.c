/* puttyalt_timeparse.c - Parse ISO-8601 and clock time strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
typedef struct { int year, month, day, hour, min, sec; } DateTime;
int tp2_iso(const char *s, DateTime *out) {
    if (!s || !out) return -1;
    int n = sscanf(s, "%d-%d-%dT%d:%d:%d", &out->year, &out->month, &out->day,
                   &out->hour, &out->min, &out->sec);
    if (n < 3) return -1;
    if (n < 6) { out->hour = out->min = out->sec = 0; }
    if (out->month < 1 || out->month > 12 || out->day < 1 || out->day > 31) return -1;
    return 0;
}
/* Days since 1970-01-01 (proleptic Gregorian), for ordering. */
long tp2_epoch_day(const DateTime *d) {
    if (!d) return 0;
    int y = d->year, m = d->month;
    long a = (14 - m) / 12;
    long yy = y + 4800 - a;
    long mm = m + 12 * a - 3;
    long jdn = d->day + (153 * mm + 2) / 5 + 365 * yy + yy/4 - yy/100 + yy/400 - 32045;
    return jdn - 2440588;
}
