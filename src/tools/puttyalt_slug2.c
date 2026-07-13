/* puttyalt_slug2.c - URL slug with max length.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <ctype.h>
int slg2_make(const char *s,int maxlen,char *out,int outlen){
    if(!s||!out)return -1;
    int o=0,dash=1;
    for(;*s&&o<outlen-1&&(maxlen<=0||o<maxlen);s++){ unsigned char c=*s; if(isalnum(c)){out[o++]=(char)tolower(c);dash=0;} else if(!dash){out[o++]='-';dash=1;} }
    while(o>0&&out[o-1]=='-')o--;
    out[o]=0;
    return o;
}
