/* puttyalt_proxyssl.c - HTTPS/CONNECT proxy tunnel handshake. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
enum ps_state { PS_INIT=0, PS_CONNECT_SENT=1, PS_ESTABLISHED=2, PS_AUTH_REQUIRED=3, PS_FAILED=4 };
typedef struct { int state; int status_code; char host[160]; int port; int has_auth; } ProxySsl;
void proxyssl_init(ProxySsl *p, const char *host, int port) {
    if(!p) return;
    memset(p,0,sizeof(*p)); snprintf(p->host,160,"%s",host?host:""); p->port=port;
}
int proxyssl_build_connect(ProxySsl *p, const char *auth, char *out, int outlen) {
    if(!p||!out) return -1;
    int n;
    if (auth && *auth) {
        n=snprintf(out,outlen,"CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\nProxy-Authorization: Basic %s\r\n\r\n",
            p->host,p->port,p->host,p->port,auth);
        p->has_auth=1;
    } else {
        n=snprintf(out,outlen,"CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n\r\n",p->host,p->port,p->host,p->port);
    }
    p->state=PS_CONNECT_SENT;
    return n;
}
int proxyssl_parse_response(ProxySsl *p, const char *response) {
    if(!p||!response) return -1;
    if (strncmp(response,"HTTP/",5)!=0) return -1;
    const char *sp=strchr(response,' '); if(!sp) return -1;
    p->status_code=atoi(sp+1);
    if (p->status_code==200) p->state=PS_ESTABLISHED;
    else if (p->status_code==407) p->state=PS_AUTH_REQUIRED;
    else p->state=PS_FAILED;
    return p->status_code;
}
int proxyssl_state(const ProxySsl *p) { return p?p->state:-1; }
