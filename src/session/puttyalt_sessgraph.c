/* puttyalt_sessgraph.c - Jump-host dependency graph for session topology. */
#include <string.h>
#include <stdio.h>
#define SG_MAX 32
#define SG_NAME 48
typedef struct { char name[SG_NAME]; int via; } sg_node; /* via = index of jump host, -1 direct */
typedef struct { sg_node n[SG_MAX]; int count; } SessGraph;
void sessgraph_init(SessGraph *g) { if(g){ memset(g,0,sizeof(*g)); } }
int sessgraph_add(SessGraph *g, const char *name, int via) {
    if(!g||g->count>=SG_MAX||!name) return -1;
    if (via>=g->count) return -1; /* via must already exist */
    snprintf(g->n[g->count].name,SG_NAME,"%s",name); g->n[g->count].via=via; return g->count++;
}
int sessgraph_chain(const SessGraph *g, int node, int *out, int cap) {
    if(!g||node<0||node>=g->count) return -1;
    int tmp[SG_MAX], depth=0, cur=node, guard=0;
    while (cur>=0 && guard++<SG_MAX) { tmp[depth++]=cur; cur=g->n[cur].via; }
    if (cur>=0) return -1; /* cycle */
    int k=depth<cap?depth:cap;
    for (int i=0;i<k;i++) out[i]=tmp[depth-1-i]; /* root first */
    return depth;
}
int sessgraph_depth(const SessGraph *g, int node) {
    int chain[SG_MAX]; return sessgraph_chain(g,node,chain,SG_MAX);
}
int sessgraph_count(const SessGraph *g) { return g?g->count:-1; }
