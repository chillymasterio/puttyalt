/* puttyalt_vtparam.c - Parse and default VT control parameters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define VTP_MAX 16
typedef struct { int params[VTP_MAX]; int n; } VtParams;
/* Parse ";"-separated numeric params with empty=>0. */
int vtp_parse(const char *s, VtParams *out) {
    if (!s || !out) return -1;
    out->n = 0;
    int cur = 0, have = 0;
    for (; ; s++) {
        if (*s >= '0' && *s <= '9') { cur = cur*10 + (*s-'0'); have = 1; }
        else {
            if (out->n < VTP_MAX) out->params[out->n++] = have ? cur : 0;
            cur = 0; have = 0;
            if (!*s) break;
        }
    }
    return out->n;
}
/* Get param i, or `dflt` if missing or zero. */
int vtp_get(const VtParams *p, int i, int dflt) {
    if (!p || i < 0 || i >= p->n) return dflt;
    return p->params[i] ? p->params[i] : dflt;
}
