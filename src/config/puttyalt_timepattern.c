/* puttyalt_timepattern.c - Format time using pattern tokens.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
typedef struct { int y,mo,d,h,mi,s; } DtVal;
/* Supports YYYY MM DD hh mm ss tokens. */
int tpat_format(const char *pat,const DtVal *v,char *out,int outlen){
    if(!pat||!v||!out)return -1;
    int o=0;
    while(*pat&&o<outlen-1){
        if(!strncmp(pat,"YYYY",4)){o+=snprintf(out+o,outlen-o,"%04d",v->y);pat+=4;}
        else if(!strncmp(pat,"MM",2)){o+=snprintf(out+o,outlen-o,"%02d",v->mo);pat+=2;}
        else if(!strncmp(pat,"DD",2)){o+=snprintf(out+o,outlen-o,"%02d",v->d);pat+=2;}
        else if(!strncmp(pat,"hh",2)){o+=snprintf(out+o,outlen-o,"%02d",v->h);pat+=2;}
        else if(!strncmp(pat,"mm",2)){o+=snprintf(out+o,outlen-o,"%02d",v->mi);pat+=2;}
        else if(!strncmp(pat,"ss",2)){o+=snprintf(out+o,outlen-o,"%02d",v->s);pat+=2;}
        else out[o++]=*pat++;
    }
    out[o]=0; return o;
}
