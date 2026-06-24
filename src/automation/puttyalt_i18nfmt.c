/* puttyalt_i18nfmt.c - Interpolate {n} placeholders in messages.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
/* Replace {0}, {1}, ... with provided argument strings. */
int if2_format(const char *tmpl, const char *const *args, int nargs, char *out, int outlen) {
    if (!tmpl || !out) return -1;
    int o = 0;
    while (*tmpl && o < outlen - 1) {
        if (*tmpl == '{' && tmpl[1] >= '0' && tmpl[1] <= '9') {
            int idx = 0; const char *p = tmpl + 1;
            while (*p >= '0' && *p <= '9') idx = idx*10 + (*p++ - '0');
            if (*p == '}') {
                if (idx < nargs && args[idx]) {
                    for (const char *a = args[idx]; *a && o < outlen-1; a++) out[o++] = *a;
                }
                tmpl = p + 1;
                continue;
            }
        }
        out[o++] = *tmpl++;
    }
    out[o] = 0;
    return o;
}
