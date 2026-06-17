/* puttyalt_searchidx.c - Inverted search index over scrollback (token -> line ids). */
#include <string.h>
#include <stdio.h>
#define SI_TERMS 128
#define SI_TERM 24
#define SI_POSTINGS 16
typedef struct { char term[SI_TERM]; int lines[SI_POSTINGS]; int nlines; } si_term;
typedef struct { si_term terms[SI_TERMS]; int n; } SearchIdx;
void searchidx_init(SearchIdx *s) { if(s) memset(s,0,sizeof(*s)); }
static si_term *si_find(SearchIdx *s, const char *term) {
    for (int i=0;i<s->n;i++) if (strcmp(s->terms[i].term,term)==0) return &s->terms[i];
    if (s->n>=SI_TERMS) return 0;
    si_term *t=&s->terms[s->n++]; snprintf(t->term,SI_TERM,"%s",term); return t;
}
int searchidx_add_line(SearchIdx *s, int line_id, const char *text) {
    if(!s||!text) return -1;
    char token[SI_TERM]; int tl=0;
    for (const char *p=text; ; p++) {
        char c=*p;
        if ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')) { if(tl<SI_TERM-1) token[tl++]=(c>='A'&&c<='Z')?c+32:c; }
        else {
            if (tl>=2) { token[tl]=0; si_term *t=si_find(s,token); if(t && t->nlines<SI_POSTINGS && (t->nlines==0||t->lines[t->nlines-1]!=line_id)) t->lines[t->nlines++]=line_id; }
            tl=0;
        }
        if (!c) break;
    }
    return 0;
}
int searchidx_query(const SearchIdx *s, const char *term, int *out_lines, int cap) {
    if(!s||!term) return -1;
    char lower[SI_TERM]; int i=0;
    for (; term[i] && i<SI_TERM-1; i++) lower[i]=(term[i]>='A'&&term[i]<='Z')?term[i]+32:term[i];
    lower[i]=0;
    for (int t=0;t<s->n;t++) if (strcmp(s->terms[t].term,lower)==0) {
        int k=s->terms[t].nlines<cap?s->terms[t].nlines:cap;
        for (int j=0;j<k;j++) out_lines[j]=s->terms[t].lines[j];
        return k;
    }
    return 0;
}
int searchidx_term_count(const SearchIdx *s) { return s?s->n:-1; }
