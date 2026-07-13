/* puttyalt_textstat.c - Compute text statistics.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
#include <string.h>
typedef struct { int chars; int letters; int digits; int spaces; int lines; } TextStat;
void txs_compute(const char *s,TextStat *out){
    if(!s||!out)return;
    memset(out,0,sizeof *out);
    for(;*s;s++){ out->chars++; unsigned char c=*s; if(isalpha(c))out->letters++; else if(isdigit(c))out->digits++; else if(isspace(c))out->spaces++; if(*s=='\n')out->lines++; }
    out->lines++;
}
