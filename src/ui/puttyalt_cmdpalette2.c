/* puttyalt_cmdpalette2.c - Command palette v2 with fuzzy scoring + recent boost. */
#include <string.h>
#include <stdio.h>
#define CP_MAX 128
#define CP_LABEL 80
typedef struct { char label[CP_LABEL]; int action_id; int recent_rank; int score; } cp2_item;
typedef struct { cp2_item items[CP_MAX]; int n; } CmdPalette2;
void cmdpalette2_init(CmdPalette2 *p) { if(p) memset(p,0,sizeof(*p)); }
int cmdpalette2_register(CmdPalette2 *p, const char *label, int action_id) {
    if(!p||p->n>=CP_MAX||!label) return -1;
    snprintf(p->items[p->n].label,CP_LABEL,"%s",label); p->items[p->n].action_id=action_id; return p->n++;
}
static int cp2_fuzzy(const char *q, const char *t) {
    int score=0, ti=0, streak=0;
    for (const char *qp=q; *qp; qp++) {
        char qc=*qp; if(qc>='A'&&qc<='Z')qc+=32;
        int found=0;
        for (; t[ti]; ti++) {
            char tc=t[ti]; if(tc>='A'&&tc<='Z')tc+=32;
            if (tc==qc) { score += 10 + streak*5; streak++; ti++; found=1; break; }
        }
        if (!found) return -1; streak=0;
    }
    return score;
}
int cmdpalette2_search(CmdPalette2 *p, const char *query, int *out_ids, int cap) {
    if(!p||!query) return -1; int nm=0;
    for (int i=0;i<p->n;i++) {
        int s = query[0]? cp2_fuzzy(query,p->items[i].label) : 0;
        if (s>=0) { p->items[i].score = s + p->items[i].recent_rank*3; } else p->items[i].score=-1;
    }
    /* selection sort by score desc into output */
    int used[CP_MAX]; memset(used,0,sizeof(used));
    for (int k=0;k<cap && k<p->n;k++) {
        int best=-1;
        for (int i=0;i<p->n;i++) if (!used[i] && p->items[i].score>=0 && (best<0||p->items[i].score>p->items[best].score)) best=i;
        if (best<0) break; used[best]=1; out_ids[nm++]=p->items[best].action_id;
    }
    return nm;
}
void cmdpalette2_mark_used(CmdPalette2 *p, int action_id) {
    if(!p) return;
    for (int i=0;i<p->n;i++) { if (p->items[i].action_id==action_id) p->items[i].recent_rank+=5; else if (p->items[i].recent_rank>0) p->items[i].recent_rank--; }
}
int cmdpalette2_count(const CmdPalette2 *p) { return p?p->n:-1; }
