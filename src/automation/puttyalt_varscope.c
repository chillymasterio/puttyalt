/* puttyalt_varscope.c - Scoped variable resolution for scripts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define VS_CAP 64
#define VS_KLEN 32
#define VS_VLEN 64
typedef struct { char key[VS_KLEN]; char val[VS_VLEN]; int scope; } VsVar;
typedef struct { VsVar v[VS_CAP]; int n; int scope; } VarScope;
void vs3_init(VarScope *s) { if (s) { s->n = 0; s->scope = 0; } }
void vs3_enter(VarScope *s) { if (s) s->scope++; }
void vs3_leave(VarScope *s) {
    if (!s || s->scope == 0) return;
    int w = 0;
    for (int i = 0; i < s->n; i++) if (s->v[i].scope < s->scope) s->v[w++] = s->v[i];
    s->n = w;
    s->scope--;
}
int vs3_set(VarScope *s, const char *k, const char *v) {
    if (!s || !k || !v) return -1;
    for (int i = 0; i < s->n; i++)
        if (s->v[i].scope == s->scope && strcmp(s->v[i].key, k) == 0) {
            strncpy(s->v[i].val, v, VS_VLEN-1); s->v[i].val[VS_VLEN-1]=0; return 0;
        }
    if (s->n >= VS_CAP) return -1;
    strncpy(s->v[s->n].key, k, VS_KLEN-1); s->v[s->n].key[VS_KLEN-1]=0;
    strncpy(s->v[s->n].val, v, VS_VLEN-1); s->v[s->n].val[VS_VLEN-1]=0;
    s->v[s->n].scope = s->scope;
    s->n++;
    return 0;
}
/* Resolve searching from innermost scope outward. */
const char *vs3_get(const VarScope *s, const char *k) {
    if (!s || !k) return 0;
    for (int sc = s->scope; sc >= 0; sc--)
        for (int i = s->n - 1; i >= 0; i--)
            if (s->v[i].scope == sc && strcmp(s->v[i].key, k) == 0) return s->v[i].val;
    return 0;
}
