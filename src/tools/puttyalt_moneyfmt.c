/* puttyalt_moneyfmt.c - Format currency amounts.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
int mny_format(long cents,char sym,char *out,int outlen){
    if(!out)return -1;
    long dollars=cents/100, c=cents%100; if(c<0)c=-c;
    char db[24]; int dl=snprintf(db,sizeof db,"%ld",dollars);
    char grouped[32]; int g=0; int first=dl%3; if(first==0)first=3;
    for(int i=0;i<dl;i++){ if(i>0&&(i-first)%3==0&&db[i-1]!='-'&&g<31)grouped[g++]=','; grouped[g++]=db[i]; }
    grouped[g]=0;
    return snprintf(out,outlen,"%c%s.%02ld",sym,grouped,c);
}
