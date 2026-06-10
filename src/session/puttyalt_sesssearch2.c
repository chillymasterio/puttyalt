/* puttyalt_sesssearch2.c - Session search v2 with multi-field ranking. */
#include <string.h>
#include <stdio.h>
#define SS_MAX 128
typedef struct { char name[64]; char host[96]; char tags[64]; char notes[128]; int last_used; } ss_sess;
typedef struct { ss_sess sessions[SS_MAX]; int n; } SessSearch2;
void sesssearch2_init(SessSearch2 *s) { if(s) memset(s,0,sizeof(*s)); }
int sesssearch2_add(SessSearch2 *s, const char *name, const char *host, const char *tags, const char *notes) {
    if(!s||s->n>=SS_MAX) return -1;
    ss_sess *e=&s->sessions[s->n++]; snprintf(e->name,64,"%s",name?name:""); snprintf(e->host,96,"%s",host?host:"");
    snprintf(e->tags,64,"%s",tags?tags:""); snprintf(e->notes,128,"%s",notes?notes:""); return 0;
}
int sesssearch2_score(const SessSearch2 *s, int idx, const char *query) {
    if(!s||idx<0||idx>=s->n||!query) return 0;
    const ss_sess *e=&s->sessions[idx]; int score=0;
    if (strstr(e->name,query)) score+=100; /* name match = highest */
    if (strstr(e->host,query)) score+=50;
    if (strstr(e->tags,query)) score+=30;
    if (strstr(e->notes,query)) score+=10;
    return score;
}
int sesssearch2_rank(const SessSearch2 *s, const char *query, int *out_idx, int cap) {
    if(!s||!query) return -1;
    int scores[SS_MAX], used[SS_MAX]={0};
    for (int i=0;i<s->n;i++) scores[i]=sesssearch2_score(s,i,query);
    int n=0;
    for (int k=0;k<cap;k++) {
        int best=-1;
        for (int i=0;i<s->n;i++) if (!used[i] && scores[i]>0 && (best<0||scores[i]>scores[best])) best=i;
        if (best<0) break; used[best]=1; out_idx[n++]=best;
    }
    return n;
}
int sesssearch2_count(const SessSearch2 *s) { return s?s->n:-1; }
