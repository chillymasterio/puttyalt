/* puttyalt_statetrack.c - Track named flags and counters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define STK_MAX 48
typedef struct { char name[32]; long value; } StkVar;
typedef struct { StkVar v[STK_MAX]; int n; } StateTrack;
void stk_init(StateTrack *s) { if (s) s->n = 0; }
static StkVar *stk_find(StateTrack *s, const char *name) {
    for (int i = 0; i < s->n; i++) if (strcmp(s->v[i].name, name) == 0) return &s->v[i];
    if (s->n < STK_MAX) { StkVar *v = &s->v[s->n++]; strncpy(v->name, name, 31); v->name[31]=0; v->value=0; return v; }
    return 0;
}
void stk_set(StateTrack *s, const char *name, long value) { if (s) { StkVar *v = stk_find(s, name); if (v) v->value = value; } }
long stk_get(StateTrack *s, const char *name) { if (!s) return 0; StkVar *v = stk_find(s, name); return v ? v->value : 0; }
void stk_incr(StateTrack *s, const char *name) { if (s) { StkVar *v = stk_find(s, name); if (v) v->value++; } }
