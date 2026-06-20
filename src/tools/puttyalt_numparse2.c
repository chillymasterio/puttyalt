/* puttyalt_numparse2.c - Parse numbers with units and signs.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <stddef.h>
/* Parse a possibly-signed integer with optional leading/trailing space.
   Sets *ok. Returns parsed value. */
long np3_int(const char *s, int *ok) {
    if (ok) *ok = 0;
    if (!s) return 0;
    while (isspace((unsigned char)*s)) s++;
    int neg = 0;
    if (*s == '-') { neg = 1; s++; } else if (*s == '+') s++;
    if (!isdigit((unsigned char)*s)) return 0;
    long v = 0;
    while (isdigit((unsigned char)*s)) v = v*10 + (*s++ - '0');
    if (ok) *ok = 1;
    return neg ? -v : v;
}
/* Parse a percentage "50%" -> 50; "50" -> 50. */
int np3_percent(const char *s, int *ok) {
    int v = (int)np3_int(s, ok);
    return v;
}
int np3_is_numeric(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '-' || *s == '+') s++;
    int digits = 0;
    for (; *s; s++) { if (!isdigit((unsigned char)*s)) return 0; digits = 1; }
    return digits;
}
