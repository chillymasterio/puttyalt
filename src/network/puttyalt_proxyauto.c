/* puttyalt_proxyauto.c - Proxy auto-config rule matcher (host -> proxy). */
#include <string.h>
#include <stdio.h>
#define PA_MAX 32
#define PA_PAT 96
#define PA_PROXY 96
typedef struct { char pattern[PA_PAT]; char proxy[PA_PROXY]; int direct; } pa_rule;
typedef struct { pa_rule r[PA_MAX]; int n; char default_proxy[PA_PROXY]; } ProxyAuto;
void proxyauto_init(ProxyAuto *p) { if(p) memset(p,0,sizeof(*p)); }
int proxyauto_add(ProxyAuto *p, const char *pattern, const char *proxy, int direct) {
    if(!p||p->n>=PA_MAX||!pattern) return -1;
    pa_rule *r=&p->r[p->n++]; snprintf(r->pattern,PA_PAT,"%s",pattern);
    snprintf(r->proxy,PA_PROXY,"%s",proxy?proxy:""); r->direct=direct?1:0; return 0;
}
static int pa_glob(const char *pat, const char *s) {
    if (pat[0]=='*') return strstr(s,pat+1)!=NULL || (pat[1]=='\0');
    return strcmp(pat,s)==0;
}
int proxyauto_resolve(const ProxyAuto *p, const char *host, char *out, int outlen) {
    if(!p||!host) return -1;
    for (int i=0;i<p->n;i++) if (pa_glob(p->r[i].pattern,host)) {
        if (p->r[i].direct) { if(out)snprintf(out,outlen,"DIRECT"); return 1; }
        if (out) snprintf(out,outlen,"%s",p->r[i].proxy);
        return 0;
    }
    if (out) snprintf(out,outlen,"%s",p->default_proxy[0]?p->default_proxy:"DIRECT");
    return p->default_proxy[0]?0:1;
}
void proxyauto_set_default(ProxyAuto *p, const char *proxy) { if(p) snprintf(p->default_proxy,PA_PROXY,"%s",proxy?proxy:""); }
