/* puttyalt_redact.c - Redact secrets from log lines.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Replace runs that look like tokens/keys with asterisks. Returns out len. */
int rdt_line(const char *in, char *out, int outlen) {
    if (!in || !out) return -1;
    int o = 0;
    for (const char *p = in; *p && o < outlen - 1; ) {
        /* detect a long alnum run (>=20) -> redact */
        const char *start = p; int run = 0;
        while (p[run] && (isalnum((unsigned char)p[run]) || p[run]=='_' || p[run]=='-' || p[run]=='+' || p[run]=='/')) run++;
        if (run >= 20) {
            int keep = 4;
            for (int i = 0; i < keep && o < outlen-1 && i < run; i++) out[o++] = start[i];
            for (int i = keep; i < run && o < outlen-1; i++) out[o++] = '*';
            p += run;
        } else {
            out[o++] = *p++;
        }
    }
    out[o] = 0;
    return o;
}
