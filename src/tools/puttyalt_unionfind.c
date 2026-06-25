/* puttyalt_unionfind.c - Union-find / disjoint set.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define UF_MAX 256
typedef struct { int parent[UF_MAX]; int rank[UF_MAX]; int n; } UnionFind;
void uf2_init(UnionFind *u, int n) {
    if (!u) return;
    u->n = (n > 0 && n <= UF_MAX) ? n : UF_MAX;
    for (int i = 0; i < u->n; i++) { u->parent[i] = i; u->rank[i] = 0; }
}
int uf2_find(UnionFind *u, int x) {
    if (!u || x < 0 || x >= u->n) return -1;
    while (u->parent[x] != x) { u->parent[x] = u->parent[u->parent[x]]; x = u->parent[x]; }
    return x;
}
void uf2_union(UnionFind *u, int a, int b) {
    int ra = uf2_find(u, a), rb = uf2_find(u, b);
    if (ra < 0 || rb < 0 || ra == rb) return;
    if (u->rank[ra] < u->rank[rb]) { int t = ra; ra = rb; rb = t; }
    u->parent[rb] = ra;
    if (u->rank[ra] == u->rank[rb]) u->rank[ra]++;
}
int uf2_connected(UnionFind *u, int a, int b) { return uf2_find(u, a) == uf2_find(u, b); }
