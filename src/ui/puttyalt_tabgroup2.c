/* puttyalt_tabgroup2.c - Tab group v2 with color labels + collapse. */
#include <string.h>
#include <stdio.h>
#define TG_MAX 16
#define TG_NAME 32
typedef struct { char name[TG_NAME]; unsigned int color; int collapsed; int tab_ids[16]; int ntabs; } tg_group;
typedef struct { tg_group g[TG_MAX]; int n; } TabGroup2;
void tabgroup2_init(TabGroup2 *t) { if(t) memset(t,0,sizeof(*t)); }
int tabgroup2_create(TabGroup2 *t, const char *name, unsigned int color) {
    if(!t||t->n>=TG_MAX||!name) return -1;
    tg_group *g=&t->g[t->n]; snprintf(g->name,TG_NAME,"%s",name); g->color=color; return t->n++;
}
int tabgroup2_add_tab(TabGroup2 *t, int group, int tab_id) {
    if(!t||group<0||group>=t->n||t->g[group].ntabs>=16) return -1;
    t->g[group].tab_ids[t->g[group].ntabs++]=tab_id; return 0;
}
void tabgroup2_toggle(TabGroup2 *t, int group) { if(t&&group>=0&&group<t->n) t->g[group].collapsed^=1; }
int tabgroup2_group_of(const TabGroup2 *t, int tab_id) {
    if(!t) return -1;
    for (int g=0;g<t->n;g++) for (int i=0;i<t->g[g].ntabs;i++) if (t->g[g].tab_ids[i]==tab_id) return g;
    return -1;
}
int tabgroup2_visible_tabs(const TabGroup2 *t, int *out, int cap) {
    if(!t) return -1; int n=0;
    for (int g=0;g<t->n;g++) if (!t->g[g].collapsed) for (int i=0;i<t->g[g].ntabs && n<cap;i++) out[n++]=t->g[g].tab_ids[i];
    return n;
}
int tabgroup2_count(const TabGroup2 *t) { return t?t->n:-1; }
