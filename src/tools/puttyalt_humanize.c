/* puttyalt_humanize.c - Humanize values (duration/count/relative time). */
#include <stdio.h>
int humanize_count(long n, char *out, int outlen) {
    if (!out) return -1;
    if (n < 1000) return snprintf(out, outlen, "%ld", n);
    if (n < 1000000) return snprintf(out, outlen, "%.1fK", n / 1000.0);
    if (n < 1000000000L) return snprintf(out, outlen, "%.1fM", n / 1000000.0);
    return snprintf(out, outlen, "%.1fB", n / 1000000000.0);
}
int humanize_duration(long seconds, char *out, int outlen) {
    if (!out) return -1;
    if (seconds < 60) return snprintf(out, outlen, "%lds", seconds);
    if (seconds < 3600) return snprintf(out, outlen, "%ldm %lds", seconds / 60, seconds % 60);
    if (seconds < 86400) return snprintf(out, outlen, "%ldh %ldm", seconds / 3600, (seconds % 3600) / 60);
    return snprintf(out, outlen, "%ldd %ldh", seconds / 86400, (seconds % 86400) / 3600);
}
int humanize_ago(long seconds_ago, char *out, int outlen) {
    if (!out) return -1;
    if (seconds_ago < 60) return snprintf(out, outlen, "just now");
    if (seconds_ago < 3600) return snprintf(out, outlen, "%ld min ago", seconds_ago / 60);
    if (seconds_ago < 86400) return snprintf(out, outlen, "%ld hr ago", seconds_ago / 3600);
    return snprintf(out, outlen, "%ld days ago", seconds_ago / 86400);
}
