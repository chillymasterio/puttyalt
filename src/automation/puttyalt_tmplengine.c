/* puttyalt_tmplengine.c - Render {{var}} templates from a var table.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
typedef struct { const char *key; const char *val; } TmplVar;
static const char *tmpl_lookup(const TmplVar *vars, int n, const char *key, int klen) {
    for (int i = 0; i < n; i++)
        if ((int)strlen(vars[i].key) == klen && strncmp(vars[i].key, key, klen) == 0)
            return vars[i].val;
    return 0;
}
int tmpl_render(const char *src, const TmplVar *vars, int nvars, char *out, int outlen) {
    if (!src || !out) return -1;
    int o = 0;
    while (*src && o < outlen - 1) {
        if (src[0] == '{' && src[1] == '{') {
            const char *end = strstr(src + 2, "}}");
            if (end) {
                const char *v = tmpl_lookup(vars, nvars, src + 2, (int)(end - src - 2));
                if (v) { int vl = (int)strlen(v); for (int k = 0; k < vl && o < outlen - 1; k++) out[o++] = v[k]; }
                src = end + 2;
                continue;
            }
        }
        out[o++] = *src++;
    }
    out[o] = 0;
    return o;
}
