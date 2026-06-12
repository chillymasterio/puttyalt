/* puttyalt_splitlayout2.c - Split layout v2 with nested binary tree. */
#include <string.h>
#define SL_MAX 32
typedef struct { int is_leaf; int pane_id; int vertical; float ratio; int child_a; int child_b; int x,y,w,h; } sl_node;
typedef struct { sl_node nodes[SL_MAX]; int n; int root; } SplitLayout2;
void splitlayout2_init(SplitLayout2 *s, int w, int h) {
    if(!s) return;
    memset(s,0,sizeof(*s));
    s->root=0; s->nodes[0].is_leaf=1; s->nodes[0].pane_id=0; s->nodes[0].x=0; s->nodes[0].y=0; s->nodes[0].w=w; s->nodes[0].h=h;
    s->n=1;
}
int splitlayout2_split(SplitLayout2 *s, int node_idx, int vertical, float ratio) {
    if(!s||node_idx<0||node_idx>=s->n||!s->nodes[node_idx].is_leaf||s->n+2>SL_MAX) return -1;
    int a=s->n++, b=s->n++;
    s->nodes[a].is_leaf=1; s->nodes[a].pane_id=s->nodes[node_idx].pane_id;
    s->nodes[b].is_leaf=1; s->nodes[b].pane_id=s->n; /* new pane id */
    s->nodes[node_idx].is_leaf=0; s->nodes[node_idx].vertical=vertical; s->nodes[node_idx].ratio=ratio;
    s->nodes[node_idx].child_a=a; s->nodes[node_idx].child_b=b;
    return b;
}
static void sl_layout(SplitLayout2 *s, int idx, int x, int y, int w, int h) {
    sl_node *n=&s->nodes[idx]; n->x=x; n->y=y; n->w=w; n->h=h;
    if (n->is_leaf) return;
    if (n->vertical) { int aw=(int)(w*n->ratio); sl_layout(s,n->child_a,x,y,aw,h); sl_layout(s,n->child_b,x+aw,y,w-aw,h); }
    else { int ah=(int)(h*n->ratio); sl_layout(s,n->child_a,x,y,w,ah); sl_layout(s,n->child_b,x,y+ah,w,h-ah); }
}
int splitlayout2_recompute(SplitLayout2 *s) {
    if(!s) return -1;
    sl_node *r=&s->nodes[s->root]; sl_layout(s,s->root,r->x,r->y,r->w,r->h); return 0;
}
int splitlayout2_leaf_count(const SplitLayout2 *s) {
    if(!s) return -1;
    int n=0; for(int i=0;i<s->n;i++) if(s->nodes[i].is_leaf)n++; return n;
}
