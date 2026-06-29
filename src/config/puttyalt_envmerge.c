/* puttyalt_envmerge.c - Merge environment variable sets.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define EM_MAX 48
typedef struct { char name[32]; char value[96]; } EmVar;
typedef struct { EmVar vars[EM_MAX]; int n; } EnvSet;
void em3_init(EnvSet *e) { if (e) e->n = 0; }
int em3_set(EnvSet *e, const char *name, const char *value) {
    if (!e || !name || !value) return -1;
    for (int i = 0; i < e->n; i++) if (strcmp(e->vars[i].name, name) == 0) {
        strncpy(e->vars[i].value, value, 95); e->vars[i].value[95] = 0; return 0;
    }
    if (e->n >= EM_MAX) return -1;
    strncpy(e->vars[e->n].name, name, 31); e->vars[e->n].name[31] = 0;
    strncpy(e->vars[e->n].value, value, 95); e->vars[e->n].value[95] = 0;
    e->n++;
    return 0;
}
/* Overlay `over` onto `base`. */
int em3_merge(EnvSet *base, const EnvSet *over) {
    if (!base || !over) return -1;
    for (int i = 0; i < over->n; i++) em3_set(base, over->vars[i].name, over->vars[i].value);
    return 0;
}
const char *em3_get(const EnvSet *e, const char *name) {
    if (!e || !name) return 0;
    for (int i = 0; i < e->n; i++) if (strcmp(e->vars[i].name, name) == 0) return e->vars[i].value;
    return 0;
}
