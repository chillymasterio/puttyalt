/* puttyalt_themeval.c - Theme JSON validation (required color keys present + valid hex). */
#include <string.h>
#include <stdio.h>
static int tv_is_hex_color(const char *v) {
    if(!v||v[0]!='#') return 0;
    int n=(int)strlen(v+1);
    if (n!=6 && n!=8) return 0;
    for (const char *p=v+1;*p;p++) if(!((*p>='0'&&*p<='9')||(*p>='a'&&*p<='f')||(*p>='A'&&*p<='F'))) return 0;
    return 1;
}
int themeval_check(const char *json, char *err, int errlen) {
    if(!json) return -1;
    static const char *req[]={"\"background\"","\"foreground\"","\"cursor\"",NULL};
    for (int i=0;req[i];i++) if (!strstr(json,req[i])) {
        if (err) snprintf(err,errlen,"missing key %s",req[i]);
        return -1;
    }
    return 0;
}
int themeval_validate_color(const char *hex) { return tv_is_hex_color(hex)?0:-1; }
int themeval_count_colors(const char *json) {
    if(!json) return 0; int n=0; for (const char *p=json;(p=strchr(p,'#'));p++) {
        char tmp[10]; int k=0; const char *q=p; while (*q && k<9 && *q!='"' && *q!=',') tmp[k++]=*q++; tmp[k]=0;
        if (tv_is_hex_color(tmp)) n++;
    }
    return n;
}
