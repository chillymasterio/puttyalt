/* puttyalt_htmlsan.c - Sanitize HTML by escaping dangerous chars.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int hts_escape(const char *s,char *out,int outlen){
    if(!s||!out)return -1;
    int o=0;
    for(;*s&&o<outlen-7;s++){ switch(*s){ case '<':memcpy(out+o,"&lt;",4);o+=4;break; case '>':memcpy(out+o,"&gt;",4);o+=4;break; case '&':memcpy(out+o,"&amp;",5);o+=5;break; case '"':memcpy(out+o,"&quot;",6);o+=6;break; default:out[o++]=*s; } }
    out[o]=0; return o;
}
