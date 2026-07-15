/* puttyalt_tooltip3.c - Rich tooltip with multi-line + key hints. */
#include <string.h>
#include <stdio.h>
#define TT_LINES 6
#define TT_LINE 80
typedef struct { char lines[TT_LINES][TT_LINE]; int n; char key_hint[24]; int x, y; int visible; } Tooltip3;
void tooltip3_init(Tooltip3 *t) { if(t) memset(t,0,sizeof(*t)); }
void tooltip3_clear(Tooltip3 *t) { if(t){ t->n=0; t->key_hint[0]=0; } }
int tooltip3_add_line(Tooltip3 *t, const char *line) {
    if(!t||t->n>=TT_LINES||!line) return -1;
    snprintf(t->lines[t->n++],TT_LINE,"%s",line); return 0;
}
void tooltip3_set_key_hint(Tooltip3 *t, const char *hint) { if(t) snprintf(t->key_hint,24,"%s",hint?hint:""); }
void tooltip3_show(Tooltip3 *t, int x, int y) { if(t){ t->x=x; t->y=y; t->visible=1; } }
void tooltip3_hide(Tooltip3 *t) { if(t) t->visible=0; }
int tooltip3_width(const Tooltip3 *t) {
    if(!t) return 0;
    int w=0;
    for (int i=0;i<t->n;i++) { int l=(int)strlen(t->lines[i]); if(l>w)w=l; }
    int kl=(int)strlen(t->key_hint); if(kl>w)w=kl;
    return w;
}
int tooltip3_height(const Tooltip3 *t) { return t? (t->n + (t->key_hint[0]?1:0)):0; }
int tooltip3_is_visible(const Tooltip3 *t) { return t?t->visible:-1; }
