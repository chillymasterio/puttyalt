/* puttyalt_urihost.c - Extract host/port/path from a URI.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
typedef struct { char scheme[12]; char host[96]; int port; char path[160]; } Uri;
int uri_parse(const char *url, Uri *out) {
    if (!url||!out) return -1;
    memset(out,0,sizeof *out);
    const char *p=url;
    const char *sep=strstr(p,"://");
    if (sep) { int sl=(int)(sep-p); if(sl>=12)sl=11; memcpy(out->scheme,p,sl); out->scheme[sl]=0; p=sep+3; }
    const char *slash=strchr(p,'/');
    const char *hostend = slash ? slash : p+strlen(p);
    const char *colon=memchr(p,':',hostend-p);
    if (colon) { int hl=(int)(colon-p); if(hl>=96)hl=95; memcpy(out->host,p,hl); out->host[hl]=0; out->port=atoi(colon+1); }
    else { int hl=(int)(hostend-p); if(hl>=96)hl=95; memcpy(out->host,p,hl); out->host[hl]=0;
           out->port = strcmp(out->scheme,"https")==0?443:(strcmp(out->scheme,"http")==0?80:0); }
    if (slash) { strncpy(out->path,slash,159); out->path[159]=0; } else strcpy(out->path,"/");
    return out->host[0]?0:-1;
}
