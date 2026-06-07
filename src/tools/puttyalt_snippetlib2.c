/* puttyalt_snippetlib2.c - Snippet library v2 with tags + fuzzy search. */
#include <string.h>
#include <stdio.h>
#define SL_MAX 64
#define SL_NAME 48
#define SL_BODY 256
#define SL_TAGS 64
typedef struct { char name[SL_NAME]; char body[SL_BODY]; char tags[SL_TAGS]; int use_count; } sl_snippet;
typedef struct { sl_snippet s[SL_MAX]; int n; } SnippetLib2;
void snippetlib2_init(SnippetLib2 *l) { if(l) memset(l,0,sizeof(*l)); }
int snippetlib2_add(SnippetLib2 *l, const char *name, const char *body, const char *tags) {
    if(!l||l->n>=SL_MAX||!name) return -1;
    sl_snippet *s=&l->s[l->n++]; snprintf(s->name,SL_NAME,"%s",name); snprintf(s->body,SL_BODY,"%s",body?body:"");
    snprintf(s->tags,SL_TAGS,"%s",tags?tags:""); return 0;
}
int snippetlib2_by_tag(const SnippetLib2 *l, const char *tag, int *out_idx, int cap) {
    if(!l||!tag) return -1; int n=0;
    for (int i=0;i<l->n && n<cap;i++) if (strstr(l->s[i].tags,tag)) out_idx[n++]=i;
    return n;
}
int snippetlib2_search(const SnippetLib2 *l, const char *query, int *out_idx, int cap) {
    if(!l||!query) return -1; int n=0;
    for (int i=0;i<l->n && n<cap;i++) if (strstr(l->s[i].name,query)||strstr(l->s[i].body,query)||strstr(l->s[i].tags,query)) out_idx[n++]=i;
    return n;
}
const char *snippetlib2_body(SnippetLib2 *l, int idx) {
    if(!l||idx<0||idx>=l->n) return 0; l->s[idx].use_count++; return l->s[idx].body;
}
int snippetlib2_count(const SnippetLib2 *l) { return l?l->n:-1; }
