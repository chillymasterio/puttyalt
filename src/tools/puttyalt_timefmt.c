/* puttyalt_timefmt.c - Timestamp formatting (relative, ISO, custom). */
#include <stdio.h>
int timefmt_relative(long seconds_ago, char *out, int outlen) {
    if(!out) return -1;
    if (seconds_ago<0) seconds_ago=0;
    if (seconds_ago<60) return snprintf(out,outlen,"%lds ago",seconds_ago);
    if (seconds_ago<3600) return snprintf(out,outlen,"%ldm ago",seconds_ago/60);
    if (seconds_ago<86400) return snprintf(out,outlen,"%ldh ago",seconds_ago/3600);
    if (seconds_ago<2592000) return snprintf(out,outlen,"%ldd ago",seconds_ago/86400);
    return snprintf(out,outlen,"%ldmo ago",seconds_ago/2592000);
}
int timefmt_iso(long y, int mo, int d, int h, int mi, int s, char *out, int outlen) {
    return snprintf(out,outlen,"%04ld-%02d-%02dT%02d:%02d:%02d",y,mo,d,h,mi,s);
}
int timefmt_clock(int total_sec, char *out, int outlen) {
    int h=total_sec/3600, m=(total_sec%3600)/60, s=total_sec%60;
    if (h>0) return snprintf(out,outlen,"%d:%02d:%02d",h,m,s);
    return snprintf(out,outlen,"%d:%02d",m,s);
}
