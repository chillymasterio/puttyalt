/* puttyalt_ipv6.c - IPv6 address parsing/formatting/classification. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct { unsigned short groups[8]; int valid; } Ipv6Addr;
int ipv6_parse(const char *str, Ipv6Addr *out) {
    if(!str||!out) return -1;
    memset(out,0,sizeof(*out));
    /* handle :: compression */
    const char *dc=strstr(str,"::");
    int left[8], right[8], nl=0, nr=0;
    if (dc) {
        char buf[64]; snprintf(buf,sizeof(buf),"%s",str);
        char *d=strstr(buf,"::"); *d=0; const char *rp=d+2;
        char *tok; char *p=buf;
        while ((tok=p) && *tok) { char *c=strchr(p,':'); if(c)*c=0; if(*tok&&nl<8) left[nl++]=(int)strtol(tok,0,16); p=c?c+1:(char*)""; if(!c)break; }
        char rbuf[64]; snprintf(rbuf,sizeof(rbuf),"%s",rp); p=rbuf;
        while (*p) { char *c=strchr(p,':'); if(c)*c=0; if(*p&&nr<8) right[nr++]=(int)strtol(p,0,16); p=c?c+1:(char*)""; if(!c)break; }
        if (nl+nr>8) return -1;
        for (int i=0;i<nl;i++) out->groups[i]=left[i];
        for (int i=0;i<nr;i++) out->groups[8-nr+i]=right[i];
    } else {
        int n=0; char buf[64]; snprintf(buf,sizeof(buf),"%s",str);
        char *bp=buf;
        while (*bp && n<8) { char *c=strchr(bp,':'); if(c)*c=0; out->groups[n++]=(int)strtol(bp,0,16); bp=c?c+1:(char*)""; if(!c)break; }
        if (n!=8) return -1;
    }
    out->valid=1; return 0;
}
int ipv6_format(const Ipv6Addr *a, char *out, int outlen) {
    if(!a||!out) return -1;
    return snprintf(out,outlen,"%x:%x:%x:%x:%x:%x:%x:%x",
        a->groups[0],a->groups[1],a->groups[2],a->groups[3],a->groups[4],a->groups[5],a->groups[6],a->groups[7]);
}
int ipv6_is_loopback(const Ipv6Addr *a) {
    if(!a) return 0;
    for(int i=0;i<7;i++) if(a->groups[i]) return 0;
    return a->groups[7]==1;
}
int ipv6_is_linklocal(const Ipv6Addr *a) { return a? ((a->groups[0]&0xFFC0)==0xFE80):0; }
