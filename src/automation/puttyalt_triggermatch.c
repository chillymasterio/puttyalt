/* puttyalt_triggermatch.c - Match output lines against trigger rules.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TM_MAX 32
enum { TM_ACT_NOTIFY, TM_ACT_LOG, TM_ACT_SOUND, TM_ACT_HIGHLIGHT };
typedef struct { char pattern[64]; int action; int hits; } TmTrigger;
typedef struct { TmTrigger t[TM_MAX]; int n; } TriggerSet;
void tm3_init(TriggerSet *s) { if (s) s->n = 0; }
int tm3_add(TriggerSet *s, const char *pattern, int action) {
    if (!s || s->n >= TM_MAX || !pattern) return -1;
    strncpy(s->t[s->n].pattern, pattern, 63); s->t[s->n].pattern[63] = 0;
    s->t[s->n].action = action; s->t[s->n].hits = 0;
    s->n++;
    return 0;
}
/* Scan a line; fire matching triggers, return count fired. */
int tm3_scan(TriggerSet *s, const char *line) {
    if (!s || !line) return 0;
    int fired = 0;
    for (int i = 0; i < s->n; i++)
        if (strstr(line, s->t[i].pattern)) { s->t[i].hits++; fired++; }
    return fired;
}
