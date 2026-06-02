/* puttyalt_settingdiff.c - Diff two settings blobs into changed-key list. */
#include <string.h>
#include <stdio.h>
#define SD_MAX 64
#define SD_KEY 48
#define SD_VAL 128
typedef struct { char key[SD_KEY]; char val[SD_VAL]; } sd_pair;
static int sd_parse(const char *blob, sd_pair *out, int cap) {
    int n=0; const char *p=blob;
    while (*p && n<cap) {
        const char *eq=strchr(p,'='); const char *nl=strchr(p,'\n');
        if (!eq || (nl && eq>nl)) { p = nl?nl+1:p+strlen(p); continue; }
        int kl=(int)(eq-p); if(kl>=SD_KEY) kl=SD_KEY-1;
        memcpy(out[n].key,p,kl); out[n].key[kl]=0;
        const char *vend = nl?nl:eq+strlen(eq);
        int vl=(int)(vend-(eq+1)); if(vl>=SD_VAL) vl=SD_VAL-1; if(vl<0)vl=0;
        memcpy(out[n].val,eq+1,vl); out[n].val[vl]=0;
        n++; p = nl?nl+1:p+strlen(p);
    }
    return n;
}
int settingdiff_compute(const char *a, const char *b, char *out, int outlen) {
    if(!a||!b||!out) return -1;
    sd_pair pa[SD_MAX], pb[SD_MAX];
    int na=sd_parse(a,pa,SD_MAX), nb=sd_parse(b,pb,SD_MAX);
    int pos=0, changes=0;
    for (int i=0;i<nb;i++) {
        const char *oldv=NULL;
        for (int j=0;j<na;j++) if (strcmp(pa[j].key,pb[i].key)==0) { oldv=pa[j].val; break; }
        if (!oldv) { pos+=snprintf(out+pos,outlen-pos,"+ %s=%s\n",pb[i].key,pb[i].val); changes++; }
        else if (strcmp(oldv,pb[i].val)!=0) { pos+=snprintf(out+pos,outlen-pos,"~ %s: %s -> %s\n",pb[i].key,oldv,pb[i].val); changes++; }
    }
    for (int i=0;i<na;i++) {
        int found=0; for (int j=0;j<nb;j++) if (strcmp(pb[j].key,pa[i].key)==0) { found=1; break; }
        if (!found) { pos+=snprintf(out+pos,outlen-pos,"- %s\n",pa[i].key); changes++; }
    }
    return changes;
}
