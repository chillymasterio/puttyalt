/* puttyalt_chordfmt.c - Format modifier+key chords.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int crd_format(int ctrl,int alt,int shift,char key,char *out,int outlen){
    if(!out)return -1;
    int o=0;
    if(ctrl){const char *m="C-";while(*m&&o<outlen-1)out[o++]=*m++;}
    if(alt){const char *m="M-";while(*m&&o<outlen-1)out[o++]=*m++;}
    if(shift){const char *m="S-";while(*m&&o<outlen-1)out[o++]=*m++;}
    if(o<outlen-1)out[o++]=key;
    out[o]=0; return o;
}
