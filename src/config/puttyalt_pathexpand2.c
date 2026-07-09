/* puttyalt_pathexpand2.c - Expand ~ and env-style path prefixes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int pxp_expand(const char *path,const char *home,char *out,int outlen){
    if(!path||!out)return -1;
    if(path[0]=='~'&&(path[1]=='/'||path[1]==0)&&home){
        int hl=(int)strlen(home); if(hl>=outlen)return -1;
        memcpy(out,home,hl); int o=hl;
        for(const char *p=path+1;*p&&o<outlen-1;p++)out[o++]=*p;
        out[o]=0;
        return o;
    }
    strncpy(out,path,outlen-1); out[outlen-1]=0; return (int)strlen(out);
}
