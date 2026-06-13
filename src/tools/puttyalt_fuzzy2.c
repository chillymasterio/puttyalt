/* puttyalt_fuzzy2.c - Fuzzy matcher v2 with scoring (fzf-style). */
#include <string.h>
int fuzzy2_score(const char *needle, const char *haystack) {
    if(!needle||!haystack) return -1;
    if (!*needle) return 0;
    int score=0, consecutive=0; const char *h=haystack; int first_match=-1, hi=0;
    for (const char *n=needle; *n; n++) {
        char nc=*n; if(nc>='A'&&nc<='Z')nc+=32;
        int found=0;
        for (; *h; h++, hi++) {
            char hc=*h; if(hc>='A'&&hc<='Z')hc+=32;
            if (hc==nc) {
                if (first_match<0) first_match=hi;
                score += 10;
                if (consecutive) score += 5;
                if (hi==0 || haystack[hi-1]=='/'||haystack[hi-1]=='_'||haystack[hi-1]==' '||haystack[hi-1]=='-') score += 8;
                /* word boundary */
                consecutive=1; h++; hi++; found=1; break;
            } else consecutive=0;
        }
        if (!found) return -1;
    }
    /* penalize leading gap */
    score -= first_match;
    return score;
}
int fuzzy2_rank(const char *needle, const char **candidates, int n, int *out_scores) {
    if(!needle||!candidates) return -1;
    int matches=0;
    for (int i=0;i<n;i++) {
        int s=fuzzy2_score(needle,candidates[i]);
        if(out_scores) out_scores[i]=s;
        if (s>=0) matches++;
    }
    return matches;
}
