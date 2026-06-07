/* puttyalt_searchpanel.c - Search panel state (query, results nav, highlight). */
#include <string.h>
#include <stdio.h>
#define SP_RESULTS 256
typedef struct { char query[128]; int results[SP_RESULTS]; int nresults; int current; int case_sensitive; int regex; int visible; } SearchPanel;
void searchpanel_init(SearchPanel *s) { if(s){ memset(s,0,sizeof(*s)); s->current=-1; } }
void searchpanel_show(SearchPanel *s) { if(s) s->visible=1; }
void searchpanel_hide(SearchPanel *s) { if(s){ s->visible=0; s->query[0]=0; s->nresults=0; s->current=-1; } }
void searchpanel_set_query(SearchPanel *s, const char *q) { if(s){ snprintf(s->query,128,"%s",q?q:""); s->nresults=0; s->current=-1; } }
int searchpanel_add_result(SearchPanel *s, int line) {
    if(!s||s->nresults>=SP_RESULTS) return -1; s->results[s->nresults++]=line;
    if (s->current<0) s->current=0; return 0;
}
int searchpanel_next(SearchPanel *s) {
    if(!s||s->nresults==0) return -1; s->current=(s->current+1)%s->nresults; return s->results[s->current];
}
int searchpanel_prev(SearchPanel *s) {
    if(!s||s->nresults==0) return -1; s->current=(s->current-1+s->nresults)%s->nresults; return s->results[s->current];
}
int searchpanel_status(const SearchPanel *s, char *buf, int buflen) {
    if(!s||!buf) return -1;
    return snprintf(buf,buflen,"%d/%d",s->nresults>0?s->current+1:0,s->nresults);
}
int searchpanel_result_count(const SearchPanel *s) { return s?s->nresults:-1; }
