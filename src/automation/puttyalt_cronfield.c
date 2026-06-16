/* puttyalt_cronfield.c - Parse individual cron schedule fields.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Test whether `value` matches a single cron field spec like a star, "5",
   "1-5", a star-slash-N step, or "1,3,5". min/max bound the field.
   Returns 1 on match. */
int crf_match(const char *spec, int value, int min, int max) {
    if (!spec) return 0;
    const char *p = spec;
    while (*p) {
        if (*p == '*') {
            p++;
            if (*p == '/') {
                int step = 0; p++;
                while (*p >= '0' && *p <= '9') step = step*10 + (*p++ - '0');
                if (step > 0 && ((value - min) % step) == 0) return 1;
            } else return 1;
        } else if (*p >= '0' && *p <= '9') {
            int a = 0;
            while (*p >= '0' && *p <= '9') a = a*10 + (*p++ - '0');
            if (*p == '-') {
                int b = 0; p++;
                while (*p >= '0' && *p <= '9') b = b*10 + (*p++ - '0');
                if (value >= a && value <= b) return 1;
            } else if (value == a) return 1;
        }
        while (*p && *p != ',') p++;
        if (*p == ',') p++;
    }
    (void)max;
    return 0;
}
