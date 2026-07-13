/* puttyalt_asciibox.c - Draw ASCII boxes around text.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int abx_top(int width,char *out,int outlen){ if(!out||width<2||outlen<width+1)return -1; int o=0; out[o++]='+'; for(int i=0;i<width-2;i++)out[o++]='-'; out[o++]='+'; out[o]=0; return o; }
int abx_row(const char *text,int width,char *out,int outlen){ if(!out||width<2)return -1; int o=0,tl=(int)strlen(text); out[o++]='|'; out[o++]=' '; for(int i=0;i<width-4;i++)out[o++]=i<tl?text[i]:' '; out[o++]=' '; out[o++]='|'; out[o]=0; return o; }
