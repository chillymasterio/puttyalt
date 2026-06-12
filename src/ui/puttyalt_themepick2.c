/* puttyalt_themepick2.c - Theme picker v2 with preview + favorite ordering. */
#include <string.h>
#include <stdio.h>
#define TP_MAX 48
#define TP_NAME 40
typedef struct { char name[TP_NAME]; unsigned int bg, fg, accent; int is_dark; int favorite; int use_count; } tp_theme;
typedef struct { tp_theme t[TP_MAX]; int n; int selected; } ThemePick2;
void themepick2_init(ThemePick2 *t) { if(t){ memset(t,0,sizeof(*t)); t->selected=-1; } }
int themepick2_add(ThemePick2 *t, const char *name, unsigned int bg, unsigned int fg, unsigned int accent, int is_dark) {
    if(!t||t->n>=TP_MAX||!name) return -1;
    tp_theme *th=&t->t[t->n]; snprintf(th->name,TP_NAME,"%s",name); th->bg=bg; th->fg=fg; th->accent=accent; th->is_dark=is_dark?1:0;
    return t->n++;
}
int themepick2_select(ThemePick2 *t, int idx) {
    if(!t||idx<0||idx>=t->n) return -1;
    t->selected=idx; t->t[idx].use_count++; return 0;
}
void themepick2_toggle_favorite(ThemePick2 *t, int idx) { if(t&&idx>=0&&idx<t->n) t->t[idx].favorite^=1; }
int themepick2_ordered(const ThemePick2 *t, int *out_idx, int cap) {
    if(!t) return -1;
    int n=0;
    /* favorites first, then by use count */
    int used[TP_MAX]; memset(used,0,sizeof(used));
    for (int k=0;k<cap && k<t->n;k++) {
        int best=-1;
        for (int i=0;i<t->n;i++) if (!used[i]) {
            if (best<0) { best=i; continue; }
            int bi=t->t[i].favorite*1000 + t->t[i].use_count;
            int bb=t->t[best].favorite*1000 + t->t[best].use_count;
            if (bi>bb) best=i;
        }
        if (best<0) break; used[best]=1; out_idx[n++]=best;
    }
    return n;
}
int themepick2_count(const ThemePick2 *t) { return t?t->n:-1; }
