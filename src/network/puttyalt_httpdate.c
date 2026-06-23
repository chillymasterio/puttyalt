/* puttyalt_httpdate.c - Parse/format HTTP date fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
static const char *HD_DOW[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char *HD_MON[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
/* Format RFC 7231 IMF-fixdate. */
int hd2_format(int dow, int day, int month, int year, int h, int m, int s, char *out, int outlen) {
    if (dow < 0 || dow > 6 || month < 1 || month > 12) return -1;
    return snprintf(out, outlen, "%s, %02d %s %04d %02d:%02d:%02d GMT",
                    HD_DOW[dow], day, HD_MON[month-1], year, h, m, s);
}
int hd2_month_index(const char *mon) {
    for (int i = 0; i < 12; i++) if (strncmp(mon, HD_MON[i], 3) == 0) return i + 1;
    return -1;
}
