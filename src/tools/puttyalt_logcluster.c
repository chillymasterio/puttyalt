/* puttyalt_logcluster.c - Log line clustering by template (Drain-lite). */
#include <string.h>
#include <stdio.h>
#define LC_MAX 32
#define LC_TMPL 160
typedef struct { char template[LC_TMPL]; int count; } lc_cluster;
typedef struct { lc_cluster c[LC_MAX]; int n; } LogCluster;
void logcluster_init(LogCluster *l) { if(l) memset(l,0,sizeof(*l)); }
/* Tokenize and replace numbers/hex with <*> to form a template. */
static void lc_templatize(const char *line, char *out, int outlen) {
    int o=0; const char *p=line; int in_num=0;
    while (*p && o<outlen-4) {
        if ((*p>='0'&&*p<='9')) { if(!in_num){ out[o++]='<'; out[o++]='*'; out[o++]='>'; in_num=1; } }
        else { in_num=0; out[o++]=*p; }
        p++;
    }
    out[o]=0;
}
int logcluster_add(LogCluster *l, const char *line) {
    if(!l||!line) return -1;
    char tmpl[LC_TMPL]; lc_templatize(line,tmpl,LC_TMPL);
    for (int i=0;i<l->n;i++) if (strcmp(l->c[i].template,tmpl)==0) { l->c[i].count++; return i; }
    if (l->n>=LC_MAX) return -1;
    snprintf(l->c[l->n].template,LC_TMPL,"%s",tmpl); l->c[l->n].count=1; return l->n++;
}
int logcluster_top(const LogCluster *l, char *buf, int buflen) {
    if(!l||!buf) return -1; int best=-1, bc=0;
    for (int i=0;i<l->n;i++) if (l->c[i].count>bc) { bc=l->c[i].count; best=i; }
    if (best<0) return -1;
    return snprintf(buf,buflen,"%s (x%d)",l->c[best].template,bc);
}
int logcluster_count(const LogCluster *l) { return l?l->n:-1; }
