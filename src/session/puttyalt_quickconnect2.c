/* puttyalt_quickconnect2.c - Quick-connect string parser (user@host:port/path?opts). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct { char user[64]; char host[160]; int port; char path[160]; int compress; char keyfile[160]; } QuickConnect2;
int quickconnect2_parse(const char *str, QuickConnect2 *out) {
    if(!str||!out) return -1;
    memset(out,0,sizeof(*out)); out->port=22;
    char work[512]; snprintf(work,sizeof(work),"%s",str);
    char *p=work;
    /* scheme:// optional */
    char *scheme=strstr(p,"://"); if (scheme) p=scheme+3;
    /* opts after ? */
    char *q=strchr(p,'?');
    if (q) { *q=0; if (strstr(q+1,"compress")) out->compress=1;
             char *kf=strstr(q+1,"key="); if(kf) snprintf(out->keyfile,160,"%s",kf+4); }
    /* path after / */
    char *slash=strchr(p,'/');
    if (slash) { *slash=0; snprintf(out->path,160,"%s",slash+1); }
    /* user@ */
    char *at=strchr(p,'@');
    if (at) { *at=0; snprintf(out->user,64,"%s",p); p=at+1; }
    /* host:port */
    char *colon=strrchr(p,':');
    if (colon) { *colon=0; int pt=atoi(colon+1); if(pt>0) out->port=pt; }
    snprintf(out->host,160,"%s",p);
    return out->host[0]?0:-1;
}
int quickconnect2_format(const QuickConnect2 *q, char *out, int outlen) {
    if(!q||!out) return -1;
    if (q->user[0]) return snprintf(out,outlen,"%s@%s:%d",q->user,q->host,q->port);
    return snprintf(out,outlen,"%s:%d",q->host,q->port);
}
