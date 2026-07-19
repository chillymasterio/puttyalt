/* puttyalt_connstring.c - Build/parse connection strings.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct { char user[48]; char host[96]; int port; } ConnString;
int cns_parse(const char *s,ConnString *out){
    if(!s||!out)return -1;
    out->user[0]=0;
    out->host[0]=0;
    out->port=22;
    const char *at=strchr(s,'@'); const char *hs=s;
    if(at){ int ul=(int)(at-s); if(ul>=48)ul=47; memcpy(out->user,s,ul); out->user[ul]=0; hs=at+1; }
    const char *colon=strchr(hs,':');
    if(colon){ int hl=(int)(colon-hs); if(hl>=96)hl=95; memcpy(out->host,hs,hl); out->host[hl]=0; out->port=atoi(colon+1); }
    else { strncpy(out->host,hs,95); out->host[95]=0; }
    return out->host[0]?0:-1;
}
int cns_build(const ConnString *c,char *out,int outlen){ if(!c||!out)return -1; if(c->user[0])return snprintf(out,outlen,"%s@%s:%d",c->user,c->host,c->port); return snprintf(out,outlen,"%s:%d",c->host,c->port); }
