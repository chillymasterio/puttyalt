/* puttyalt_yamllist.c - Parse simple YAML list items.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <ctype.h>
/* Extract a "- value" list item; returns 1 if line is a list item. */
int yml_item(const char *line,char *out,int outlen){
    if(!line||!out)return 0;
    while(*line==' ')line++;
    if(*line!='-'||line[1]!=' ')return 0;
    line+=2; while(*line==' ')line++;
    strncpy(out,line,outlen-1); out[outlen-1]=0;
    int n=(int)strlen(out); while(n>0&&isspace((unsigned char)out[n-1]))out[--n]=0;
    return 1;
}
int yml_indent(const char *line){ int n=0; while(line[n]==' ')n++; return n; }
