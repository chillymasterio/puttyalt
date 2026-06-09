/* puttyalt_regex2.c - Regex engine v2 (anchors, classes, quantifiers). */
#include <string.h>
static int rx_match_here(const char *re, const char *text);
static int rx_match_star(int c, const char *re, const char *text) {
    do { if (rx_match_here(re,text)) return 1; } while (*text && (*text++==c || c=='.'));
    return 0;
}
static int rx_match_class(const char *re, char c, const char **re_end) {
    int neg=0; const char *p=re+1;
    if (*p=='^') { neg=1; p++; }
    int matched=0;
    while (*p && *p!=']') {
        if (p[1]=='-' && p[2]!=']') { if(c>=p[0]&&c<=p[2]) matched=1; p+=3; }
        else { if(c==*p) matched=1; p++; }
    }
    if (*p==']') p++;
    *re_end=p; return matched!=neg;
}
static int rx_match_here(const char *re, const char *text) {
    if (re[0]=='\0') return 1;
    if (re[0]=='$' && re[1]=='\0') return *text=='\0';
    if (re[0]=='[') {
        const char *re_end; int m=rx_match_class(re,*text,&re_end);
        if (re_end[0]=='*') return rx_match_star(-2,re,text); /* approx */
        if (*text && m) return rx_match_here(re_end,text+1);
        return 0;
    }
    if (re[1]=='*') return rx_match_star(re[0],re+2,text);
    if (re[1]=='+') { if(*text&&(re[0]=='.'||*text==re[0])) return rx_match_star(re[0],re+2,text+1); return 0; }
    if (re[1]=='?') { if(*text&&(re[0]=='.'||*text==re[0])&&rx_match_here(re+2,text+1)) return 1; return rx_match_here(re+2,text); }
    if (*text && (re[0]=='.' || re[0]==*text)) return rx_match_here(re+1,text+1);
    return 0;
}
int regex2_match(const char *pattern, const char *text) {
    if(!pattern||!text) return 0;
    if (pattern[0]=='^') return rx_match_here(pattern+1,text);
    do { if (rx_match_here(pattern,text)) return 1; } while (*text++);
    return 0;
}
int regex2_search(const char *pattern, const char *text, int *start) {
    if(!pattern||!text) return 0;
    const char *p = pattern[0]=='^' ? pattern+1 : pattern;
    int anchored = pattern[0]=='^';
    for (const char *t=text; *t||t==text; t++) {
        if (rx_match_here(p,t)) { if(start)*start=(int)(t-text); return 1; }
        if (anchored) break;
        if (!*t) break;
    }
    return 0;
}
