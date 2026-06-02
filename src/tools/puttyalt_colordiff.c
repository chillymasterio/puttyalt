/* puttyalt_colordiff.c - Side-by-side text diff line classifier (LCS-lite). */
#include <string.h>
enum cd_op { CD_SAME=0, CD_ADD=1, CD_DEL=2, CD_MOD=3 };
int colordiff_classify_line(const char *a, const char *b) {
    if (!a && !b) return CD_SAME;
    if (!a) return CD_ADD;
    if (!b) return CD_DEL;
    if (strcmp(a,b)==0) return CD_SAME;
    return CD_MOD;
}
int colordiff_similarity_pct(const char *a, const char *b) {
    if (!a||!b) return 0;
    int la=(int)strlen(a), lb=(int)strlen(b);
    if (la==0&&lb==0) return 100;
    int common=0, i=0, j=0;
    while (i<la && j<lb) { if (a[i]==b[j]) { common++; i++; j++; } else if (la-i>lb-j) i++; else j++; }
    int maxl=la>lb?la:lb;
    return maxl? (common*100)/maxl : 0;
}
