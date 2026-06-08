/* puttyalt_sessfilter.c - Session list filter (fuzzy name/host/tag combined). */
#include <string.h>
#include <stdio.h>
#define SF_MAX 128
typedef struct { char name[64]; char host[96]; char tags[64]; } sf_session;
typedef struct { sf_session s[SF_MAX]; int n; } SessFilter;
void sessfilter_init(SessFilter *f) { if(f) memset(f,0,sizeof(*f)); }
int sessfilter_add(SessFilter *f, const char *name, const char *host, const char *tags) {
    if(!f||f->n>=SF_MAX) return -1;
    sf_session *s=&f->s[f->n++]; snprintf(s->name,64,"%s",name?name:""); snprintf(s->host,96,"%s",host?host:"");
    snprintf(s->tags,64,"%s",tags?tags:""); return 0;
}
static int sf_fuzzy(const char *needle, const char *hay) {
    if(!*needle) return 1;
    const char *h=hay;
    for (const char *n=needle;*n;n++) {
        char nc=*n; if(nc>='A'&&nc<='Z')nc+=32;
        int found=0;
        for (;*h;h++) { char hc=*h; if(hc>='A'&&hc<='Z')hc+=32; if(hc==nc){h++;found=1;break;} }
        if (!found) return 0;
    }
    return 1;
}
int sessfilter_match(const SessFilter *f, const char *query, int *out_idx, int cap) {
    if(!f||!query) return -1; int n=0;
    for (int i=0;i<f->n && n<cap;i++) {
        if (sf_fuzzy(query,f->s[i].name)||sf_fuzzy(query,f->s[i].host)||sf_fuzzy(query,f->s[i].tags)) out_idx[n++]=i;
    }
    return n;
}
int sessfilter_count(const SessFilter *f) { return f?f->n:-1; }
