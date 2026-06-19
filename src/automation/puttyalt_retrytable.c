/* puttyalt_retrytable.c - Track retry counts per operation key.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define RT_MAX 32
typedef struct { int key; int attempts; int max; } RtEntry;
typedef struct { RtEntry e[RT_MAX]; int n; int default_max; } RetryTable;
void rt3_init(RetryTable *t, int default_max) { if (t) { t->n = 0; t->default_max = default_max > 0 ? default_max : 3; } }
static RtEntry *rt3_find(RetryTable *t, int key) {
    for (int i = 0; i < t->n; i++) if (t->e[i].key == key) return &t->e[i];
    if (t->n < RT_MAX) { RtEntry *e = &t->e[t->n++]; e->key = key; e->attempts = 0; e->max = t->default_max; return e; }
    return 0;
}
/* Returns 1 if a retry is allowed (and records the attempt), else 0. */
int rt3_should_retry(RetryTable *t, int key) {
    if (!t) return 0;
    RtEntry *e = rt3_find(t, key);
    if (!e || e->attempts >= e->max) return 0;
    e->attempts++;
    return 1;
}
void rt3_reset(RetryTable *t, int key) {
    if (!t) return;
    RtEntry *e = rt3_find(t, key);
    if (e) e->attempts = 0;
}
