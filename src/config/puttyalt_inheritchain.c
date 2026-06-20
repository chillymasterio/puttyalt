/* puttyalt_inheritchain.c - Resolve config inheritance chains.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define INH_MAX 16
typedef struct { char name[32]; char parent[32]; } InhNode;
typedef struct { InhNode nodes[INH_MAX]; int n; } InheritChain;
void inh_init(InheritChain *c) { if (c) c->n = 0; }
int inh_add(InheritChain *c, const char *name, const char *parent) {
    if (!c || c->n >= INH_MAX || !name) return -1;
    strncpy(c->nodes[c->n].name, name, 31); c->nodes[c->n].name[31] = 0;
    strncpy(c->nodes[c->n].parent, parent ? parent : "", 31); c->nodes[c->n].parent[31] = 0;
    c->n++;
    return 0;
}
/* Resolve the chain from `name` to root; writes names into `chain`. */
int inh_resolve(const InheritChain *c, const char *name, char chain[][32], int maxdepth) {
    if (!c || !name) return 0;
    int depth = 0;
    const char *cur = name;
    while (cur && *cur && depth < maxdepth) {
        strncpy(chain[depth], cur, 31); chain[depth][31] = 0;
        depth++;
        const char *next = 0;
        for (int i = 0; i < c->n; i++)
            if (strcmp(c->nodes[i].name, cur) == 0) { next = c->nodes[i].parent; break; }
        if (!next || !*next) break;
        /* cycle guard */
        int seen = 0;
        for (int d = 0; d < depth; d++) if (strcmp(chain[d], next) == 0) seen = 1;
        if (seen) break;
        cur = next;
    }
    return depth;
}
