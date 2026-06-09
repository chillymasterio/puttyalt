/* puttyalt_searchhl2.c - Search result highlighting with match ranges. */
#include <string.h>
#define SH_MAX 64
typedef struct { int line; int col_start; int col_end; int is_current; } sh_match;
typedef struct { sh_match matches[SH_MAX]; int n; int current; } SearchHl2;
void searchhl2_init(SearchHl2 *s) { if(s){ memset(s,0,sizeof(*s)); s->current=-1; } }
int searchhl2_add(SearchHl2 *s, int line, int col_start, int col_end) {
    if(!s||s->n>=SH_MAX) return -1;
    sh_match *m=&s->matches[s->n++]; m->line=line; m->col_start=col_start; m->col_end=col_end;
    if (s->current<0) { s->current=0; s->matches[0].is_current=1; }
    return s->n-1;
}
int searchhl2_next(SearchHl2 *s) {
    if(!s||s->n==0) return -1;
    if (s->current>=0) s->matches[s->current].is_current=0;
    s->current=(s->current+1)%s->n; s->matches[s->current].is_current=1;
    return s->current;
}
int searchhl2_prev(SearchHl2 *s) {
    if(!s||s->n==0) return -1;
    if (s->current>=0) s->matches[s->current].is_current=0;
    s->current=(s->current-1+s->n)%s->n; s->matches[s->current].is_current=1;
    return s->current;
}
int searchhl2_at(const SearchHl2 *s, int line, int col) {
    if(!s) return -1;
    for (int i=0;i<s->n;i++) if (s->matches[i].line==line && col>=s->matches[i].col_start && col<s->matches[i].col_end)
        return s->matches[i].is_current ? 2 : 1;
    return 0;
}
void searchhl2_clear(SearchHl2 *s) { if(s){ s->n=0; s->current=-1; } }
int searchhl2_count(const SearchHl2 *s) { return s?s->n:-1; }
