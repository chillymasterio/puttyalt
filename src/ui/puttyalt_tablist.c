/* puttyalt_tablist.c - Tab list model with reorder, close, activate. */
#include <string.h>
#include <stdio.h>
#define TL_MAX 32
#define TL_TITLE 64
typedef struct { char title[TL_TITLE]; int session_id; int modified; int active; } tl_tab;
typedef struct { tl_tab tabs[TL_MAX]; int n; int current; } TabList;
void tablist_init(TabList *t) { if(t){ memset(t,0,sizeof(*t)); t->current=-1; } }
int tablist_add(TabList *t, const char *title, int session_id) {
    if(!t||t->n>=TL_MAX) return -1;
    tl_tab *tab=&t->tabs[t->n]; snprintf(tab->title,TL_TITLE,"%s",title?title:""); tab->session_id=session_id;
    if (t->current<0) t->current=t->n;
    return t->n++;
}
int tablist_close(TabList *t, int idx) {
    if(!t||idx<0||idx>=t->n) return -1;
    memmove(&t->tabs[idx],&t->tabs[idx+1],sizeof(tl_tab)*(t->n-idx-1)); t->n--;
    if (t->current>=t->n) t->current=t->n-1;
    if (t->current==idx && idx<t->n) {} /* current shifts to next */
    return 0;
}
int tablist_move(TabList *t, int from, int to) {
    if(!t||from<0||from>=t->n||to<0||to>=t->n) return -1;
    tl_tab tmp=t->tabs[from];
    if (from<to) memmove(&t->tabs[from],&t->tabs[from+1],sizeof(tl_tab)*(to-from));
    else memmove(&t->tabs[to+1],&t->tabs[to],sizeof(tl_tab)*(from-to));
    t->tabs[to]=tmp; return 0;
}
void tablist_activate(TabList *t, int idx) { if(t&&idx>=0&&idx<t->n) t->current=idx; }
int tablist_next(TabList *t) { if(!t||t->n==0) return -1; t->current=(t->current+1)%t->n; return t->current; }
int tablist_count(const TabList *t) { return t?t->n:-1; }
