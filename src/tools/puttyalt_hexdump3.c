/* puttyalt_hexdump3.c - Format a compact hex dump line.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <ctype.h>
int hxd_line(const unsigned char *data,int len,int offset,char *out,int outlen){
    if(!data||!out)return -1;
    int o=snprintf(out,outlen,"%06x  ",offset);
    for(int i=0;i<16;i++){ if(i<len)o+=snprintf(out+o,outlen-o,"%02x ",data[offset+i]); else o+=snprintf(out+o,outlen-o,"   "); }
    o+=snprintf(out+o,outlen-o," ");
    for(int i=0;i<16&&i<len;i++){ unsigned char c=data[offset+i]; out[o++]=isprint(c)?(char)c:'.'; }
    out[o]=0; return o;
}
