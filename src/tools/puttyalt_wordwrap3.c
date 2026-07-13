/* puttyalt_wordwrap3.c - Wrap words with hyphenation-free breaks.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int ww4_next_break(const char *s,int width){
    if(!s||width<=0)return 0;
    int len=(int)strlen(s);
    if(len<=width)return len;
    int brk=-1;
    for(int i=0;i<=width&&i<len;i++)if(s[i]==' ')brk=i;
    return brk>0?brk:width;
}
