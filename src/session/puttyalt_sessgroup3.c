/* puttyalt_sessgroup3.c - Session group v3 with nesting + drag reorder. */
#include <string.h>
#include <stdio.h>
#define SG_MAX 48
#define SG_NAME 48
typedef struct { char name[SG_NAME]; int parent; int order; int collapsed; unsigned int color; } sg_group;
typedef struct { sg_group g[SG_MAX]; int n; } SessGroup3;
void sessgroup3_init(SessGroup3 *s) { if(s) memset(s,0,sizeof(*s)); }
int sessgroup3_create(SessGroup3 *s, const char *name, int parent, unsigned int color) {
    if(!s||s->n>=SG_MAX||!name) return -1;
    if (parent>=s->n) return -1;
    /* cycle check */
    int p=parent, guard=0; while(p>=0 && guard++<SG_MAX) { p=s->g[p].parent; }
    sg_group *g=&s->g[s->n]; snprintf(g->name,SG_NAME,"%s",name); g->parent=parent>=0?parent:-1; g->color=color; g->order=s->n;
    return s->n++;
}
int sessgroup3_reorder(SessGroup3 *s, int idx, int new_order) {
    if(!s||idx<0||idx>=s->n) return -1;
    s->g[idx].order=new_order; return 0;
}
int sessgroup3_move(SessGroup3 *s, int idx, int new_parent) {
    if(!s||idx<0||idx>=s->n) return -1;
    /* prevent cycle: new_parent can't be descendant of idx */
    int p=new_parent, guard=0;
    while (p>=0 && guard++<SG_MAX) { if (p==idx) return -1; p=s->g[p].parent; }
    s->g[idx].parent=new_parent; return 0;
}
int sessgroup3_depth(const SessGroup3 *s, int idx) {
    if(!s||idx<0||idx>=s->n) return -1;
    int d=0, p=s->g[idx].parent, guard=0; while(p>=0&&guard++<SG_MAX){ d++; p=s->g[p].parent; } return d;
}
int sessgroup3_count(const SessGroup3 *s) { return s?s->n:-1; }
