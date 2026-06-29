/* puttyalt_commandpalette2.c - Filter and rank command palette entries.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
#define CP_MAX 64
typedef struct { char name[40]; int id; int score; } CpCommand;
typedef struct { CpCommand cmds[CP_MAX]; int n; } CommandPalette2;
void cp5_init(CommandPalette2 *p) { if (p) p->n = 0; }
int cp5_add(CommandPalette2 *p, const char *name, int id) {
    if (!p || p->n >= CP_MAX || !name) return -1;
    strncpy(p->cmds[p->n].name, name, 39); p->cmds[p->n].name[39] = 0;
    p->cmds[p->n].id = id; p->cmds[p->n].score = 0;
    return p->n++;
}
static int cp5_fuzzy(const char *q, const char *name) {
    int score = 0; const char *n = name;
    for (; *q; q++) {
        while (*n && tolower((unsigned char)*n) != tolower((unsigned char)*q)) n++;
        if (!*n) return -1;
        score++; n++;
    }
    return score;
}
/* Score all commands against query; returns number with matches. */
int cp5_filter(CommandPalette2 *p, const char *query) {
    if (!p) return 0;
    int matches = 0;
    for (int i = 0; i < p->n; i++) {
        p->cmds[i].score = (query && *query) ? cp5_fuzzy(query, p->cmds[i].name) : 0;
        if (p->cmds[i].score >= 0) matches++;
    }
    return matches;
}
int cp5_best(const CommandPalette2 *p) {
    if (!p) return -1;
    int best = -1, bs = -1;
    for (int i = 0; i < p->n; i++) if (p->cmds[i].score > bs) { bs = p->cmds[i].score; best = i; }
    return best >= 0 && bs >= 0 ? p->cmds[best].id : -1;
}
