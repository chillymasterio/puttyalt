/* puttyalt_sftppath.c - Normalize SFTP remote paths.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int sfp_normalize(const char *path,char *out,int outlen){
    if(!path||!out)return -1;
    char parts[32][48]; int np=0;
    int absolute=path[0]=='/';
    const char *p=path;
    while(*p){ while(*p=='/')p++; if(!*p)break; char seg[48]; int sl=0; while(*p&&*p!='/'&&sl<47)seg[sl++]=*p++; seg[sl]=0;
        if(!strcmp(seg,"."))continue;
        if(!strcmp(seg,"..")){if(np>0)np--;continue;} if(np<32)strcpy(parts[np++],seg);
        }
    int o=0; if(absolute&&o<outlen-1)out[o++]='/';
    for(int i=0;i<np;i++){ if(i&&o<outlen-1)out[o++]='/'; for(const char *q=parts[i];*q&&o<outlen-1;q++)out[o++]=*q; }
    if(o==0&&o<outlen-1)out[o++]='.';
    out[o]=0;
    return o;
}
