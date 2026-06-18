/* puttyalt_qparams.c - Parse URL query-string parameters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define QP_MAX 32
typedef struct { char key[40]; char val[96]; } QParam;
typedef struct { QParam p[QP_MAX]; int n; } QueryParams;
int qp_parse(const char *query, QueryParams *out) {
    if (!query || !out) return -1;
    out->n = 0;
    if (*query == '?') query++;
    const char *p = query;
    while (*p && out->n < QP_MAX) {
        QParam *q = &out->p[out->n];
        int o = 0;
        while (*p && *p != '=' && *p != '&' && o < 39) q->key[o++] = *p++;
        q->key[o] = 0;
        o = 0;
        if (*p == '=') { p++; while (*p && *p != '&' && o < 95) q->val[o++] = *p++; }
        q->val[o] = 0;
        out->n++;
        if (*p == '&') p++;
    }
    return out->n;
}
const char *qp_get(const QueryParams *qp, const char *key) {
    if (!qp || !key) return 0;
    for (int i = 0; i < qp->n; i++) if (strcmp(qp->p[i].key, key) == 0) return qp->p[i].val;
    return 0;
}
