/* puttyalt_httpcache.c - HTTP cache freshness evaluation.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
typedef struct { long max_age; int no_cache; int no_store; int must_revalidate; } CacheControl;
/* Parse a Cache-Control header. */
int hc3_parse(const char *header, CacheControl *out) {
    if (!header || !out) return -1;
    out->max_age = -1; out->no_cache = 0; out->no_store = 0; out->must_revalidate = 0;
    if (strstr(header, "no-cache")) out->no_cache = 1;
    if (strstr(header, "no-store")) out->no_store = 1;
    if (strstr(header, "must-revalidate")) out->must_revalidate = 1;
    const char *ma = strstr(header, "max-age=");
    if (ma) out->max_age = atol(ma + 8);
    return 0;
}
/* Is a cached response still fresh? */
int hc3_is_fresh(const CacheControl *cc, long age_seconds) {
    if (!cc) return 0;
    if (cc->no_store || cc->no_cache) return 0;
    if (cc->max_age < 0) return 0;
    return age_seconds < cc->max_age;
}
