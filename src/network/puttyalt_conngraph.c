/* puttyalt_conngraph.c - Connection topology graph (hosts + edges + reachability). */
#include <string.h>
#include <stdio.h>
#define CG_NODES 32
#define CG_NAME 48
typedef struct { char host[CG_NAME]; int reachable; int hop_count; } cg_node;
typedef struct { cg_node nodes[CG_NODES]; int n; int adj[CG_NODES][CG_NODES]; } ConnGraph;
void conngraph_init(ConnGraph *g) { if(g) memset(g,0,sizeof(*g)); }
int conngraph_add_node(ConnGraph *g, const char *host) {
    if(!g||g->n>=CG_NODES||!host) return -1;
    snprintf(g->nodes[g->n].host,CG_NAME,"%s",host); return g->n++;
}
int conngraph_add_edge(ConnGraph *g, int from, int to) {
    if(!g||from<0||from>=g->n||to<0||to>=g->n) return -1;
    g->adj[from][to]=1; g->adj[to][from]=1; return 0;
}
int conngraph_bfs_reachable(ConnGraph *g, int start) {
    if(!g||start<0||start>=g->n) return -1;
    int queue[CG_NODES], qh=0, qt=0, visited[CG_NODES]={0};
    queue[qt++]=start; visited[start]=1; g->nodes[start].hop_count=0; g->nodes[start].reachable=1;
    int count=1;
    while (qh<qt) {
        int cur=queue[qh++];
        for (int i=0;i<g->n;i++) if (g->adj[cur][i] && !visited[i]) {
            visited[i]=1; g->nodes[i].reachable=1; g->nodes[i].hop_count=g->nodes[cur].hop_count+1;
            queue[qt++]=i; count++;
        }
    }
    return count;
}
int conngraph_node_count(const ConnGraph *g) { return g?g->n:-1; }
