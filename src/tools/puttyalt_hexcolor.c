/* puttyalt_hexcolor.c - Parse and build hex color strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <stdint.h>
int hxc_parse(const char *s,uint8_t *r,uint8_t *g,uint8_t *b){
    if(!s)return -1;
    if(*s=='#')s++;
    unsigned rv,gv,bv;
    if(sscanf(s,"%02x%02x%02x",&rv,&gv,&bv)!=3)return -1;
    if(r)*r=rv;
    if(g)*g=gv;
    if(b)*b=bv;
    return 0;
}
int hxc_build(uint8_t r,uint8_t g,uint8_t b,char *out,int outlen){ return snprintf(out,outlen,"#%02x%02x%02x",r,g,b); }
