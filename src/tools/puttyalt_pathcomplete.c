/* puttyalt_pathcomplete.c - Remote path tab-completion candidate matcher. */
#include <string.h>
#include <stdio.h>
#define PC_MAX 64
#define PC_NAME 128
typedef struct { char names[PC_MAX][PC_NAME]; int n; } PathComplete;
void pathcomplete_init(PathComplete *p) { if(p) memset(p,0,sizeof(*p)); }
int pathcomplete_add(PathComplete *p, const char *name) {
    if(!p||p->n>=PC_MAX||!name) return -1; snprintf(p->names[p->n++],PC_NAME,"%s",name); return 0;
}
int pathcomplete_match(const PathComplete *p, const char *prefix, char *out, int outlen) {
    if(!p||!prefix) return -1; int pl=(int)strlen(prefix), matches=0, first=-1;
    for (int i=0;i<p->n;i++) if (strncmp(p->names[i],prefix,pl)==0) { if(first<0)first=i; matches++; }
    if (matches==0) return 0;
    if (matches==1) { if(out) snprintf(out,outlen,"%s",p->names[first]); return 1; }
    /* longest common prefix among matches */
    char lcp[PC_NAME]; snprintf(lcp,PC_NAME,"%s",p->names[first]);
    for (int i=first+1;i<p->n;i++) if (strncmp(p->names[i],prefix,pl)==0) {
        int k=0; while (lcp[k] && p->names[i][k] && lcp[k]==p->names[i][k]) k++; lcp[k]=0;
    }
    if (out) snprintf(out,outlen,"%s",lcp);
    return matches;
}
int pathcomplete_count(const PathComplete *p) { return p?p->n:-1; }
