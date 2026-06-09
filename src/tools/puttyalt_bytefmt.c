/* puttyalt_bytefmt.c - Byte/number formatting (SI vs binary, grouping). */
#include <stdio.h>
#include <string.h>
int bytefmt_si(unsigned long long bytes, char *out, int outlen) {
    const char *u[]={"B","kB","MB","GB","TB","PB"};
    double v=(double)bytes; int i=0;
    while (v>=1000 && i<5) { v/=1000; i++; }
    if (i==0) return snprintf(out,outlen,"%llu %s",bytes,u[0]);
    return snprintf(out,outlen,"%.2f %s",v,u[i]);
}
int bytefmt_binary(unsigned long long bytes, char *out, int outlen) {
    const char *u[]={"B","KiB","MiB","GiB","TiB","PiB"};
    double v=(double)bytes; int i=0;
    while (v>=1024 && i<5) { v/=1024; i++; }
    if (i==0) return snprintf(out,outlen,"%llu %s",bytes,u[0]);
    return snprintf(out,outlen,"%.2f %s",v,u[i]);
}
int bytefmt_grouped(long long n, char *out, int outlen) {
    char tmp[32]; int len=snprintf(tmp,sizeof(tmp),"%lld",n<0?-n:n);
    int o=0; if(n<0&&o<outlen-1) out[o++]='-';
    for (int i=0;i<len && o<outlen-1;i++) {
        if (i>0 && (len-i)%3==0) out[o++]=',';
        out[o++]=tmp[i];
    }
    out[o]=0; return o;
}
