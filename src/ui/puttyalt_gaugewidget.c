/* puttyalt_gaugewidget.c - Circular/bar gauge percentage widget.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
int gwt_bar(int value,int max,int width,char *out,int outlen){
    if(!out||width<2||outlen<width+1)return -1;
    if(max<=0)max=1;
    if(value<0)value=0;
    if(value>max)value=max;
    int filled=value*width/max,o=0;
    for(int i=0;i<width&&o<outlen-1;i++)out[o++]=i<filled?'=':' ';
    out[o]=0; return o;
}
int gwt_percent(int value,int max){ return max>0?value*100/max:0; }
