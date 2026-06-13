/* puttyalt_validate2.c - Input validation (hostname/port/path/email). */
#include <string.h>
#include <stdlib.h>
int validate2_hostname(const char *host) {
    if(!host||!*host) return 0;
    int len=(int)strlen(host); if(len>253) return 0;
    int label_len=0;
    for (const char *p=host;*p;p++) {
        if (*p=='.') { if(label_len==0) return 0; label_len=0; }
        else if ((*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||(*p>='0'&&*p<='9')||*p=='-') {
            if (++label_len>63) return 0;
            if (*p=='-' && (label_len==1||p[1]=='.'||p[1]==0)) return 0;
            /* no leading/trailing hyphen */
        } else return 0;
    }
    return label_len>0;
}
int validate2_port(const char *port) {
    if(!port||!*port) return 0;
    for (const char *p=port;*p;p++) if(*p<'0'||*p>'9') return 0;
    int n=atoi(port); return (n>=1 && n<=65535);
}
int validate2_email(const char *email) {
    if(!email) return 0;
    const char *at=strchr(email,'@'); if(!at||at==email) return 0;
    const char *dot=strchr(at,'.'); if(!dot||dot==at+1||!dot[1]) return 0;
    return 1;
}
int validate2_path(const char *path) {
    if(!path||!*path) return 0;
    /* no null bytes already guaranteed; check for obvious invalid chars */
    for (const char *p=path;*p;p++) if (*p=='\0'||*p<32) return 0;
    return 1;
}
