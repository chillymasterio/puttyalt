/* puttyalt_scrollmark.c - Scrollback marks/bookmarks for quick navigation. */
#include <string.h>
#include <stdio.h>
#define SM_MAX 32
#define SM_LABEL 32
typedef struct { int line; char label[SM_LABEL]; int auto_mark; } sm_mark;
typedef struct { sm_mark marks[SM_MAX]; int n; int current; } ScrollMark;
void scrollmark_init(ScrollMark *s) { if(s){ memset(s,0,sizeof(*s)); s->current=-1; } }
int scrollmark_add(ScrollMark *s, int line, const char *label, int auto_mark) {
    if(!s||s->n>=SM_MAX) return -1;
    sm_mark *m=&s->marks[s->n++]; m->line=line; snprintf(m->label,SM_LABEL,"%s",label?label:""); m->auto_mark=auto_mark?1:0;
    return s->n-1;
}
int scrollmark_next(ScrollMark *s, int from_line) {
    if(!s) return -1; int best=-1, bl=1<<30;
    for (int i=0;i<s->n;i++) if (s->marks[i].line>from_line && s->marks[i].line<bl) { bl=s->marks[i].line; best=i; }
    if (best>=0) s->current=best; return best>=0?s->marks[best].line:-1;
}
int scrollmark_prev(ScrollMark *s, int from_line) {
    if(!s) return -1; int best=-1, bl=-1;
    for (int i=0;i<s->n;i++) if (s->marks[i].line<from_line && s->marks[i].line>bl) { bl=s->marks[i].line; best=i; }
    if (best>=0) s->current=best; return best>=0?s->marks[best].line:-1;
}
int scrollmark_count(const ScrollMark *s) { return s?s->n:-1; }
void scrollmark_clear_auto(ScrollMark *s) {
    if(!s) return; int w=0; for(int i=0;i<s->n;i++) if(!s->marks[i].auto_mark) s->marks[w++]=s->marks[i]; s->n=w;
}
