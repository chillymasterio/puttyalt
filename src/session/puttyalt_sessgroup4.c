/* puttyalt_sessgroup4.c - Session group v4 with color tags + counts. */
#include <string.h>
#include <stdio.h>
#define SG_MAX 24
#define SG_NAME 48
typedef struct {
    char name[SG_NAME];
    unsigned int color;
    int member_count;
    int parent;
    int collapsed;
} sg_group;
typedef struct {
    sg_group groups[SG_MAX];
    int n;
} SessGroup4;
void sessgroup4_init(SessGroup4 *s) {
    if (s) memset(s, 0, sizeof(*s));
}
int sessgroup4_create(SessGroup4 *s, const char *name, unsigned int color, int parent) {
    if (!s || s->n >= SG_MAX || !name) return -1;
    if (parent >= s->n) return -1;
    sg_group *g = &s->groups[s->n];
    snprintf(g->name, SG_NAME, "%s", name);
    g->color = color;
    g->parent = parent >= 0 ? parent : -1;
    return s->n++;
}
void sessgroup4_add_member(SessGroup4 *s, int group) {
    if (s && group >= 0 && group < s->n) s->groups[group].member_count++;
}
void sessgroup4_remove_member(SessGroup4 *s, int group) {
    if (s && group >= 0 && group < s->n && s->groups[group].member_count > 0) {
        s->groups[group].member_count--;
    }
}
int sessgroup4_total_members(const SessGroup4 *s, int group) {
    if (!s || group < 0 || group >= s->n) return -1;
    int total = s->groups[group].member_count;
    for (int i = 0; i < s->n; i++) {
        if (s->groups[i].parent == group) total += sessgroup4_total_members(s, i);
    }
    return total;
}
int sessgroup4_count(const SessGroup4 *s) {
    return s ? s->n : -1;
}
