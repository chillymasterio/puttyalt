/* puttyalt_gpurender.c - GPU render frame graph: pass ordering + resource barriers. */
#include <string.h>
#include <stdio.h>
#define GR_MAX_PASSES 16
#define GR_NAME 32
enum gr_pass_type { GR_CLEAR=0, GR_TEXT=1, GR_OVERLAY=2, GR_CURSOR=3, GR_PRESENT=4 };
typedef struct { char name[GR_NAME]; int type; int reads_mask; int writes_mask; int order; } gr_pass;
typedef struct { gr_pass passes[GR_MAX_PASSES]; int n; int frame; } GpuRender;
void gpurender_init(GpuRender *g) { if(g) memset(g,0,sizeof(*g)); }
int gpurender_add_pass(GpuRender *g, const char *name, int type, int reads, int writes) {
    if(!g||g->n>=GR_MAX_PASSES) return -1;
    gr_pass *p=&g->passes[g->n]; snprintf(p->name,GR_NAME,"%s",name?name:""); p->type=type;
    p->reads_mask=reads; p->writes_mask=writes; p->order=g->n; return g->n++;
}
int gpurender_schedule(GpuRender *g, int *out_order, int cap) {
    if(!g) return -1;
    /* topological-ish: passes that write a resource before passes that read it; PRESENT last */
    int ord[GR_MAX_PASSES], k=0;
    for (int i=0;i<g->n;i++) if (g->passes[i].type!=GR_PRESENT) ord[k++]=i;
    /* stable sort by writes-before-reads via order field already insertion order */
    for (int i=0;i<g->n;i++) if (g->passes[i].type==GR_PRESENT) ord[k++]=i;
    int m=k<cap?k:cap;
    for (int i=0;i<m;i++) out_order[i]=ord[i];
    return m;
}
int gpurender_needs_barrier(const GpuRender *g, int pass_a, int pass_b) {
    if(!g||pass_a<0||pass_b<0||pass_a>=g->n||pass_b>=g->n) return 0;
    /* barrier needed if pass_a writes what pass_b reads */
    return (g->passes[pass_a].writes_mask & g->passes[pass_b].reads_mask)?1:0;
}
void gpurender_next_frame(GpuRender *g) { if(g) g->frame++; }
int gpurender_pass_count(const GpuRender *g) { return g?g->n:-1; }
