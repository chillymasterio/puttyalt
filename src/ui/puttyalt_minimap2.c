/* puttyalt_minimap2.c - Scrollback minimap v2: density buckets + viewport marker. */
#include <string.h>
#define MM_BUCKETS 64
typedef struct { int density[MM_BUCKETS]; int total_lines; int viewport_top; int viewport_height; } Minimap2;
void minimap2_init(Minimap2 *m) { if(m) memset(m,0,sizeof(*m)); }
void minimap2_set_lines(Minimap2 *m, int total) { if(m) m->total_lines=total<0?0:total; }
void minimap2_set_viewport(Minimap2 *m, int top, int height) { if(m){ m->viewport_top=top; m->viewport_height=height; } }
int minimap2_line_to_bucket(const Minimap2 *m, int line) {
    if(!m||m->total_lines==0) return 0;
    int b=(line*MM_BUCKETS)/m->total_lines; return b>=MM_BUCKETS?MM_BUCKETS-1:(b<0?0:b);
}
void minimap2_mark_line(Minimap2 *m, int line, int weight) {
    if(!m) return; int b=minimap2_line_to_bucket(m,line); m->density[b]+=weight;
}
int minimap2_viewport_bucket_range(const Minimap2 *m, int *first, int *last) {
    if(!m) return -1;
    if (first) *first=minimap2_line_to_bucket(m,m->viewport_top);
    if (last) *last=minimap2_line_to_bucket(m,m->viewport_top+m->viewport_height);
    return 0;
}
int minimap2_density(const Minimap2 *m, int bucket) { return (m&&bucket>=0&&bucket<MM_BUCKETS)?m->density[bucket]:-1; }
