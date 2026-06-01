/* puttyalt_durparse.c - Parse human durations ("1h30m", "45s", "2d") to ms. */
#include <string.h>
#include <stdio.h>
long long durparse_ms(const char *s) {
    if(!s) return -1;
    long long total=0, num=0; int seen=0;
    for (const char *p=s; *p; p++) {
        if (*p>='0'&&*p<='9') { num=num*10+(*p-'0'); seen=1; }
        else {
            long long mult=0;
            switch(*p) { case 'd':mult=86400000LL;break; case 'h':mult=3600000LL;break;
                case 'm':mult=60000LL;break; case 's':mult=1000LL;break; default:return -1; }
            total+=num*mult; num=0;
        }
    }
    if (num>0) total+=num*1000LL; /* bare number = seconds */
    return seen?total:-1;
}
int durparse_format(long long ms, char *buf, int buflen) {
    if (ms<0) return -1;
    long long d=ms/86400000; ms%=86400000;
    long long h=ms/3600000; ms%=3600000;
    long long m=ms/60000; ms%=60000;
    long long s=ms/1000;
    int pos=0;
    if (d) pos+=snprintf(buf+pos,buflen-pos,"%lldd",d);
    if (h) pos+=snprintf(buf+pos,buflen-pos,"%lldh",h);
    if (m) pos+=snprintf(buf+pos,buflen-pos,"%lldm",m);
    if (s||pos==0) pos+=snprintf(buf+pos,buflen-pos,"%llds",s);
    return pos;
}
