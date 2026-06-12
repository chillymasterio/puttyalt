/* puttyalt_textdiff.c - Line-based text diff (LCS algorithm). */
#include <string.h>
#define TD_MAX 128
enum td_op { TD_EQUAL=0, TD_INSERT=1, TD_DELETE=2 };
typedef struct { int op; int a_line; int b_line; } td_edit;
typedef struct { td_edit edits[TD_MAX*2]; int n; } TextDiff;
void textdiff_init(TextDiff *t) { if(t) memset(t,0,sizeof(*t)); }
/* Simple diff: compare line arrays, mark equal/insert/delete */
int textdiff_compute(TextDiff *t, const char **a, int na, const char **b, int nb) {
    if(!t||!a||!b||na>TD_MAX||nb>TD_MAX) return -1; t->n=0;
    /* LCS length table */
    static int lcs[TD_MAX+1][TD_MAX+1];
    for (int i=0;i<=na;i++) for (int j=0;j<=nb;j++) {
        if (i==0||j==0) lcs[i][j]=0;
        else if (strcmp(a[i-1],b[j-1])==0) lcs[i][j]=lcs[i-1][j-1]+1;
        else lcs[i][j]=lcs[i-1][j]>lcs[i][j-1]?lcs[i-1][j]:lcs[i][j-1];
    }
    /* backtrack */
    int i=na, j=nb; td_edit tmp[TD_MAX*2]; int tn=0;
    while (i>0 && j>0) {
        if (strcmp(a[i-1],b[j-1])==0) { tmp[tn].op=TD_EQUAL; tmp[tn].a_line=i-1; tmp[tn].b_line=j-1; tn++; i--; j--; }
        else if (lcs[i-1][j]>=lcs[i][j-1]) { tmp[tn].op=TD_DELETE; tmp[tn].a_line=i-1; tmp[tn].b_line=-1; tn++; i--; }
        else { tmp[tn].op=TD_INSERT; tmp[tn].a_line=-1; tmp[tn].b_line=j-1; tn++; j--; }
    }
    while (i>0) { tmp[tn].op=TD_DELETE; tmp[tn].a_line=i-1; tmp[tn].b_line=-1; tn++; i--; }
    while (j>0) { tmp[tn].op=TD_INSERT; tmp[tn].a_line=-1; tmp[tn].b_line=j-1; tn++; j--; }
    for (int k=0;k<tn;k++) t->edits[k]=tmp[tn-1-k];
    t->n=tn; return tn;
}
int textdiff_changes(const TextDiff *t) {
    if(!t) return -1; int n=0; for(int i=0;i<t->n;i++) if(t->edits[i].op!=TD_EQUAL)n++; return n;
}
int textdiff_count(const TextDiff *t) { return t?t->n:-1; }
