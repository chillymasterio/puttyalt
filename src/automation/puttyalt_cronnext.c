/* puttyalt_cronnext.c - Compute next cron fire time (minute granularity).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Check if minute/hour match simple cron fields (-1 = wildcard). */
int cn2_matches(int cron_min, int cron_hour, int min, int hour) {
    if (cron_min >= 0 && cron_min != min) return 0;
    if (cron_hour >= 0 && cron_hour != hour) return 0;
    return 1;
}
/* Minutes until the next match from (cur_hour,cur_min), scanning 24h. */
int cn2_minutes_until(int cron_min, int cron_hour, int cur_hour, int cur_min) {
    for (int delta = 1; delta <= 1440; delta++) {
        int total = (cur_hour * 60 + cur_min + delta) % 1440;
        int h = total / 60, m = total % 60;
        if (cn2_matches(cron_min, cron_hour, m, h)) return delta;
    }
    return -1;
}
