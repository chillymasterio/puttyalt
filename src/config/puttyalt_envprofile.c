/* puttyalt_envprofile.c - Per-environment variable profile (dev/staging/prod). */
#include <string.h>
#include <stdio.h>
#define EP_MAX 32
#define EP_NAME 40
#define EP_VAL 160
typedef struct { char key[EP_NAME]; char val[EP_VAL]; } ep_var;
typedef struct { char profile[EP_NAME]; ep_var v[EP_MAX]; int n; } EnvProfile;
void envprofile_init(EnvProfile *e, const char *profile) {
    if(!e) return; memset(e,0,sizeof(*e)); snprintf(e->profile,EP_NAME,"%s",profile?profile:"default");
}
int envprofile_set(EnvProfile *e, const char *key, const char *val) {
    if(!e||!key) return -1;
    for (int i=0;i<e->n;i++) if (strcmp(e->v[i].key,key)==0) { snprintf(e->v[i].val,EP_VAL,"%s",val?val:""); return 0; }
    if (e->n>=EP_MAX) return -1;
    snprintf(e->v[e->n].key,EP_NAME,"%s",key); snprintf(e->v[e->n].val,EP_VAL,"%s",val?val:""); e->n++; return 0;
}
int envprofile_expand(const EnvProfile *e, const char *tmpl, char *out, int outlen) {
    if(!e||!tmpl||!out) return -1; int o=0;
    for (const char *p=tmpl; *p && o<outlen-1; ) {
        if (*p=='$' && p[1]=='{') {
            const char *end=strchr(p+2,'}');
            if (end) {
                char key[EP_NAME]; int kl=(int)(end-(p+2)); if(kl>=EP_NAME)kl=EP_NAME-1;
                memcpy(key,p+2,kl); key[kl]=0;
                for (int i=0;i<e->n;i++) if (strcmp(e->v[i].key,key)==0) { o+=snprintf(out+o,outlen-o,"%s",e->v[i].val); break; }
                p=end+1; continue;
            }
        }
        out[o++]=*p++;
    }
    out[o]=0; return o;
}
int envprofile_count(const EnvProfile *e) { return e?e->n:-1; }
