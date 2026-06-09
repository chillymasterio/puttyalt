/* puttyalt_gauge.c - Gauge/meter widget renderer with color zones. */
#include <string.h>
#include <stdio.h>
typedef struct { int min, max, value; int warn_threshold; int crit_threshold; int width; } Gauge;
void gauge_init(Gauge *g, int min, int max, int width) {
    if(!g) return; memset(g,0,sizeof(*g)); g->min=min; g->max=max; g->width=width>0?width:20;
    g->warn_threshold=max*7/10; g->crit_threshold=max*9/10;
}
void gauge_set_value(Gauge *g, int value) {
    if(!g) return; g->value = value<g->min?g->min:(value>g->max?g->max:value);
}
int gauge_zone(const Gauge *g) {
    if(!g) return 0;
    if (g->value>=g->crit_threshold) return 2; /* critical */
    if (g->value>=g->warn_threshold) return 1; /* warning */
    return 0; /* normal */
}
int gauge_render(const Gauge *g, char *out, int outlen) {
    if(!g||!out) return -1;
    int range=g->max-g->min; if(range==0)range=1;
    int filled=(g->value-g->min)*g->width/range;
    int pos=0; out[pos++]='[';
    for (int i=0;i<g->width && pos<outlen-1;i++) out[pos++]= i<filled?'=':' ';
    pos+=snprintf(out+pos,outlen-pos,"] %d%%",(g->value-g->min)*100/range);
    return pos;
}
int gauge_percent(const Gauge *g) {
    if(!g) return 0; int range=g->max-g->min; if(range==0)return 0; return (g->value-g->min)*100/range;
}
