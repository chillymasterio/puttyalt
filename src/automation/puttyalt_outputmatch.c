/* puttyalt_outputmatch.c - Match terminal output against expectations.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define OM_MAX 16
typedef struct { char pattern[64]; int seen; } OmExpect;
typedef struct { OmExpect e[OM_MAX]; int n; int all_seen; } OutputMatch;
void om2_init(OutputMatch *m) { if (m) { m->n = 0; m->all_seen = 0; } }
int om2_expect(OutputMatch *m, const char *pattern) {
    if (!m || m->n >= OM_MAX || !pattern) return -1;
    strncpy(m->e[m->n].pattern, pattern, 63); m->e[m->n].pattern[63] = 0;
    m->e[m->n].seen = 0;
    m->n++;
    return 0;
}
/* Feed a line; mark matched expectations. Returns 1 when all seen. */
int om2_feed(OutputMatch *m, const char *line) {
    if (!m || !line) return 0;
    for (int i = 0; i < m->n; i++) if (!m->e[i].seen && strstr(line, m->e[i].pattern)) m->e[i].seen = 1;
    int all = 1;
    for (int i = 0; i < m->n; i++) if (!m->e[i].seen) all = 0;
    m->all_seen = all;
    return all;
}
