/* puttyalt_breadcrumb2.c - Breadcrumb navigation v2 with clickable segments. */
#include <string.h>
#include <stdio.h>
#define BC_MAX 12
#define BC_SEG 48
typedef struct { char label[BC_SEG]; int node_id; int x_start, x_end; } bc_seg;
typedef struct { bc_seg segs[BC_MAX]; int n; } Breadcrumb2;
void breadcrumb2_init(Breadcrumb2 *b) { if(b) memset(b,0,sizeof(*b)); }
int breadcrumb2_push(Breadcrumb2 *b, const char *label, int node_id) {
    if(!b||b->n>=BC_MAX||!label) return -1;
    bc_seg *s=&b->segs[b->n++]; snprintf(s->label,BC_SEG,"%s",label); s->node_id=node_id; return 0;
}
int breadcrumb2_pop_to(Breadcrumb2 *b, int seg_idx) {
    if(!b||seg_idx<0||seg_idx>=b->n) return -1; b->n=seg_idx+1; return b->segs[seg_idx].node_id;
}
int breadcrumb2_layout(Breadcrumb2 *b, int start_x, int sep_width) {
    if(!b) return 0; int x=start_x;
    for (int i=0;i<b->n;i++) {
        if (i>0) x+=sep_width;
        b->segs[i].x_start=x; x+=(int)strlen(b->segs[i].label); b->segs[i].x_end=x;
    }
    return x;
}
int breadcrumb2_hit(const Breadcrumb2 *b, int x) {
    if(!b) return -1;
    for (int i=0;i<b->n;i++) if (x>=b->segs[i].x_start && x<b->segs[i].x_end) return i;
    return -1;
}
int breadcrumb2_render(const Breadcrumb2 *b, char *out, int outlen) {
    if(!b||!out) return -1; int pos=0;
    for (int i=0;i<b->n && pos<outlen;i++) pos+=snprintf(out+pos,outlen-pos,"%s%s",i?" > ":"",b->segs[i].label);
    return pos;
}
int breadcrumb2_count(const Breadcrumb2 *b) { return b?b->n:-1; }
