/* puttyalt_pluralize2.c - Pluralize English nouns.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdio.h>
int plz_form(const char *noun,int count,char *out,int outlen){
    if(!noun||!out)return -1;
    int n=(int)strlen(noun);
    if(count==1){ strncpy(out,noun,outlen-1); out[outlen-1]=0; return (int)strlen(out); }
    if(n>=1&&(noun[n-1]=='s'||noun[n-1]=='x'||noun[n-1]=='z'))return snprintf(out,outlen,"%ses",noun);
    if(n>=1&&noun[n-1]=='y'&&n>=2&&!strchr("aeiou",noun[n-2])){ int o=n-1; if(o>=outlen-4)o=outlen-4; memcpy(out,noun,o); return o+snprintf(out+o,outlen-o,"ies"); }
    return snprintf(out,outlen,"%ss",noun);
}
