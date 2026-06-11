/* puttyalt_urlparse.c - URL parser (scheme/host/port/path/query). */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct { char scheme[16]; char host[128]; int port; char path[160]; char query[128]; char user[64]; } UrlParse;
int urlparse(const char *url, UrlParse *out) {
    if(!url||!out) return -1; memset(out,0,sizeof(*out)); out->port=0;
    char work[512]; snprintf(work,sizeof(work),"%s",url);
    char *p=work;
    char *scheme=strstr(p,"://");
    if (scheme) { int sl=(int)(scheme-p); if(sl<15){ memcpy(out->scheme,p,sl); out->scheme[sl]=0; } p=scheme+3; }
    char *q=strchr(p,'?'); if(q){ *q=0; snprintf(out->query,128,"%s",q+1); }
    char *path=strchr(p,'/'); if(path){ snprintf(out->path,160,"%s",path); *path=0; }
    char *at=strchr(p,'@'); if(at){ *at=0; snprintf(out->user,64,"%s",p); p=at+1; }
    char *colon=strrchr(p,':'); if(colon){ *colon=0; out->port=atoi(colon+1); }
    snprintf(out->host,128,"%s",p);
    if (out->port==0) {
        if (strcmp(out->scheme,"https")==0) out->port=443;
        else if (strcmp(out->scheme,"http")==0) out->port=80;
        else if (strcmp(out->scheme,"ssh")==0) out->port=22;
        else if (strcmp(out->scheme,"ftp")==0) out->port=21;
    }
    return out->host[0]?0:-1;
}
int urlparse_is_secure(const UrlParse *u) {
    if(!u) return 0; return (strcmp(u->scheme,"https")==0||strcmp(u->scheme,"ssh")==0||strcmp(u->scheme,"sftp")==0)?1:0;
}
