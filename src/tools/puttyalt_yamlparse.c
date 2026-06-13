/* puttyalt_yamlparse.c - Minimal flat YAML parser (key: value, no nesting). */
#include <string.h>
#include <stdio.h>
int yamlparse_get(const char *yaml, const char *key, char *out, int outlen) {
    if(!yaml||!key||!out) return -1;
    const char *line=yaml;
    int kl=(int)strlen(key);
    while (*line) {
        const char *nl=strchr(line,'\n');
        const char *p=line; while(*p==' '||*p=='\t')p++;
        if (strncmp(p,key,kl)==0 && p[kl]==':') {
            const char *v=p+kl+1; while(*v==' '||*v=='\t')v++;
            const char *ve=nl?nl:v+strlen(v);
            int vl=(int)(ve-v); if(vl>=outlen)vl=outlen-1;
            /* strip quotes */
            if (vl>=2 && (v[0]=='"'||v[0]=='\'')) { v++; vl-=2; }
            memcpy(out,v,vl>0?vl:0); out[vl>0?vl:0]=0;
            /* trim trailing */
            int l=(int)strlen(out); while(l>0&&(out[l-1]==' '||out[l-1]=='\r'))out[--l]=0;
            return l;
        }
        if(!nl) break;
        line=nl+1;
    }
    return -1;
}
int yamlparse_has_key(const char *yaml, const char *key) {
    char tmp[256]; return yamlparse_get(yaml,key,tmp,sizeof(tmp))>=0?1:0;
}
