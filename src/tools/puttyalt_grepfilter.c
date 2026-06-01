/* puttyalt_grepfilter.c - Live output grep filter (include/exclude patterns). */
#include <string.h>
#include <stdio.h>
#define GF_MAX 16
#define GF_PAT 96
typedef struct { char inc[GF_MAX][GF_PAT]; int ninc; char exc[GF_MAX][GF_PAT]; int nexc; int case_insens; } GrepFilter;
void grepfilter_init(GrepFilter *g) { if(g) memset(g,0,sizeof(*g)); }
int grepfilter_add_include(GrepFilter *g, const char *pat) {
    if(!g||g->ninc>=GF_MAX||!pat) return -1; snprintf(g->inc[g->ninc++],GF_PAT,"%s",pat); return 0;
}
int grepfilter_add_exclude(GrepFilter *g, const char *pat) {
    if(!g||g->nexc>=GF_MAX||!pat) return -1; snprintf(g->exc[g->nexc++],GF_PAT,"%s",pat); return 0;
}
static int gf_contains(const char *hay, const char *needle, int ci) {
    if (!ci) return strstr(hay,needle)!=NULL;
    size_t nl=strlen(needle);
    for (const char *p=hay; *p; p++) {
        size_t i=0; for (; i<nl; i++) { char a=p[i], b=needle[i]; if(a>='A'&&a<='Z')a+=32; if(b>='A'&&b<='Z')b+=32; if(a!=b)break; }
        if (i==nl) return 1;
    }
    return 0;
}
int grepfilter_matches(const GrepFilter *g, const char *line) {
    if(!g||!line) return 1;
    for (int i=0;i<g->nexc;i++) if (gf_contains(line,g->exc[i],g->case_insens)) return 0;
    if (g->ninc==0) return 1;
    for (int i=0;i<g->ninc;i++) if (gf_contains(line,g->inc[i],g->case_insens)) return 1;
    return 0;
}
void grepfilter_set_case_insensitive(GrepFilter *g, int ci) { if(g) g->case_insens=ci?1:0; }
