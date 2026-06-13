/* puttyalt_configdiff.c - Config version diff + 3-way merge for sync. */
#include <string.h>
#include <stdio.h>
#define CD_MAX 64
#define CD_KEY 40
#define CD_VAL 96
typedef struct { char key[CD_KEY]; char val[CD_VAL]; } cd_kv;
static int cd_parse(const char *blob, cd_kv *out, int cap) {
    int n=0; const char *p=blob;
    while (*p && n<cap) {
        const char *eq=strchr(p,'='), *nl=strchr(p,'\n');
        if (eq && (!nl||eq<nl)) {
            int kl=(int)(eq-p); if(kl>=CD_KEY)kl=CD_KEY-1; memcpy(out[n].key,p,kl); out[n].key[kl]=0;
            const char *ve=nl?nl:eq+strlen(eq); int vl=(int)(ve-(eq+1)); if(vl>=CD_VAL)vl=CD_VAL-1; if(vl<0)vl=0;
            memcpy(out[n].val,eq+1,vl); out[n].val[vl]=0; n++;
        }
        if(!nl) break;
        p=nl+1;
    }
    return n;
}
static const char *cd_get(cd_kv *kv, int n, const char *key) {
    for(int i=0;i<n;i++) if(strcmp(kv[i].key,key)==0) return kv[i].val; return 0;
}
int configdiff_merge(const char *base, const char *local, const char *remote, char *out, int outlen) {
    cd_kv b[CD_MAX], l[CD_MAX], r[CD_MAX];
    int nb=cd_parse(base,b,CD_MAX), nl=cd_parse(local,l,CD_MAX), nr=cd_parse(remote,r,CD_MAX);
    int pos=0, conflicts=0;
    for (int i=0;i<nl;i++) {
        const char *bv=cd_get(b,nb,l[i].key), *rv=cd_get(r,nr,l[i].key);
        const char *chosen=l[i].val;
        int lchg = !bv||strcmp(bv,l[i].val)!=0;
        int rchg = rv && (!bv||strcmp(bv,rv)!=0);
        if (lchg && rchg && strcmp(l[i].val,rv?rv:"")!=0) { conflicts++; chosen=l[i].val; } /* local wins, count conflict */
        else if (rchg && !lchg) chosen=rv;
        pos+=snprintf(out+pos,outlen-pos,"%s=%s\n",l[i].key,chosen);
    }
    return conflicts;
}
