/* puttyalt_levenshtein.c - Levenshtein edit distance (for typo suggestions). */
#include <string.h>
int levenshtein(const char *a, const char *b) {
    if(!a||!b) return -1;
    int la=(int)strlen(a), lb=(int)strlen(b);
    if (la==0) return lb; if (lb==0) return la;
    if (la>127||lb>127) return -1;
    int prev[128], cur[128];
    for (int j=0;j<=lb;j++) prev[j]=j;
    for (int i=1;i<=la;i++) {
        cur[0]=i;
        for (int j=1;j<=lb;j++) {
            int cost = a[i-1]==b[j-1] ? 0 : 1;
            int del=prev[j]+1, ins=cur[j-1]+1, sub=prev[j-1]+cost;
            int m=del<ins?del:ins; cur[j]=m<sub?m:sub;
        }
        memcpy(prev,cur,sizeof(int)*(lb+1));
    }
    return prev[lb];
}
int levenshtein_closest(const char *target, const char **candidates, int n, int *out_dist) {
    if(!target||!candidates) return -1;
    int best=-1, bd=1<<30;
    for (int i=0;i<n;i++) { int d=levenshtein(target,candidates[i]); if(d>=0&&d<bd){bd=d;best=i;} }
    if (out_dist) *out_dist=bd;
    return best;
}
int levenshtein_similar(const char *a, const char *b, int max_dist) {
    int d=levenshtein(a,b); return (d>=0 && d<=max_dist)?1:0;
}
