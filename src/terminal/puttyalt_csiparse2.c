/* puttyalt_csiparse2.c - Parse CSI control sequences.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define CSI_MAXP 16
typedef struct { int params[CSI_MAXP]; int nparams; char final; int priv; } CsiSeq;
/* Parse the body of an ESC[ sequence (after the '['). Returns 0 on success. */
int csi2_parse(const char *body, CsiSeq *out) {
    if (!body || !out) return -1;
    out->nparams = 0; out->priv = 0; out->final = 0;
    const char *p = body;
    if (*p == '?') { out->priv = 1; p++; }
    int cur = 0, have = 0;
    for (; *p; p++) {
        if (*p >= '0' && *p <= '9') { cur = cur*10 + (*p - '0'); have = 1; }
        else if (*p == ';') {
            if (out->nparams < CSI_MAXP) out->params[out->nparams++] = have ? cur : 0;
            cur = 0; have = 0;
        } else if (*p >= 0x40 && *p <= 0x7e) {
            if (have || out->nparams == 0) { if (out->nparams < CSI_MAXP) out->params[out->nparams++] = have ? cur : 0; }
            out->final = *p;
            return 0;
        }
    }
    return -1;
}
int csi2_param(const CsiSeq *s, int i, int dflt) {
    if (!s || i < 0 || i >= s->nparams) return dflt;
    return s->params[i];
}
