/* puttyalt_guardrail.c - Block dangerous commands by pattern.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define GD_MAX 16
typedef struct { char pattern[48]; } GdRule;
typedef struct { GdRule rules[GD_MAX]; int n; } Guardrail;
void gd3_init(Guardrail *g) {
    if (!g) return;
    g->n = 0;
    /* sensible defaults */
    const char *def[] = {"rm -rf /", "mkfs", "dd if=", ":(){ :|:& };:", "> /dev/sd"};
    for (unsigned i = 0; i < sizeof(def)/sizeof(def[0]) && g->n < GD_MAX; i++) {
        strncpy(g->rules[g->n].pattern, def[i], 47); g->rules[g->n].pattern[47]=0; g->n++;
    }
}
int gd3_add(Guardrail *g, const char *pattern) {
    if (!g || g->n >= GD_MAX || !pattern) return -1;
    strncpy(g->rules[g->n].pattern, pattern, 47); g->rules[g->n].pattern[47]=0;
    g->n++;
    return 0;
}
/* Returns index of matched dangerous rule, or -1 if safe. */
int gd3_check(const Guardrail *g, const char *command) {
    if (!g || !command) return -1;
    for (int i = 0; i < g->n; i++) if (strstr(command, g->rules[i].pattern)) return i;
    return -1;
}
