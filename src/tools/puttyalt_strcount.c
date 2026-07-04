/* puttyalt_strcount.c - Count occurrences and characters.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int scnt_substr(const char *s, const char *sub){
    if(!s||!sub||!*sub)return 0;
    int c=0,sl=(int)strlen(sub);
    for(const char *p=s;(p=strstr(p,sub));p+=sl)c++;
    return c;
}
int scnt_char(const char *s, char c){ int n=0; for(;*s;s++)if(*s==c)n++; return n; }
int scnt_words(const char *s){ int n=0,in=0; for(;*s;s++){ if(*s==' '||*s=='\t'||*s=='\n')in=0; else if(!in){in=1;n++;} } return n; }
