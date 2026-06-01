/* puttyalt_byteunit.c - Human-readable byte/rate formatting (KiB/MiB/GiB). */
#include <stdio.h>
int byteunit_format(unsigned long long bytes, char *buf, int buflen) {
    const char *u[]={"B","KiB","MiB","GiB","TiB"};
    double v=(double)bytes; int i=0;
    while (v>=1024.0 && i<4) { v/=1024.0; i++; }
    if (i==0) return snprintf(buf,buflen,"%llu %s",bytes,u[0]);
    return snprintf(buf,buflen,"%.1f %s",v,u[i]);
}
int byteunit_rate(unsigned long long bytes, double secs, char *buf, int buflen) {
    if (secs<=0) return snprintf(buf,buflen,"-- /s");
    unsigned long long bps=(unsigned long long)(bytes/secs);
    char tmp[32]; byteunit_format(bps,tmp,sizeof(tmp));
    return snprintf(buf,buflen,"%s/s",tmp);
}
