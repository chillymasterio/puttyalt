/* puttyalt_glob.c - Glob pattern matching (*, ?, [...]). */
#include <string.h>
int glob_match(const char *pattern, const char *str) {
    if(!pattern||!str) return 0;
    while (*pattern) {
        if (*pattern=='*') {
            pattern++;
            if (!*pattern) return 1;
            while (*str) { if (glob_match(pattern,str)) return 1; str++; }
            return glob_match(pattern,str);
        } else if (*pattern=='?') {
            if (!*str) return 0;
            pattern++; str++;
        } else if (*pattern=='[') {
            pattern++; int neg=0, matched=0;
            if (*pattern=='!'||*pattern=='^') { neg=1; pattern++; }
            while (*pattern && *pattern!=']') {
                if (pattern[1]=='-' && pattern[2]!=']') {
                    if (*str>=pattern[0] && *str<=pattern[2]) matched=1;
                    pattern+=3;
                } else { if (*str==*pattern) matched=1; pattern++; }
            }
            if (*pattern==']') pattern++;
            if (matched==neg || !*str) return 0;
            str++;
        } else {
            if (*pattern!=*str) return 0;
            pattern++; str++;
        }
    }
    return *str==0;
}
int glob_match_ci(const char *pattern, const char *str) {
    char lp[256], ls[256]; int i;
    for(i=0;pattern[i]&&i<255;i++) lp[i]=(pattern[i]>='A'&&pattern[i]<='Z')?pattern[i]+32:pattern[i]; lp[i]=0;
    for(i=0;str[i]&&i<255;i++) ls[i]=(str[i]>='A'&&str[i]<='Z')?str[i]+32:str[i]; ls[i]=0;
    return glob_match(lp,ls);
}
