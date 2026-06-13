/* puttyalt_iprange.c - IP range / CIDR matching utility. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
static uint32_t ipr_parse(const char *ip) {
    uint32_t a=0,b=0,c=0,d=0;
    if (sscanf(ip,"%u.%u.%u.%u",&a,&b,&c,&d)!=4) return 0;
    return (a<<24)|(b<<16)|(c<<8)|d;
}
int iprange_in_cidr(const char *ip, const char *cidr) {
    if(!ip||!cidr) return 0;
    char net[64]; snprintf(net,sizeof(net),"%s",cidr);
    char *slash=strchr(net,'/'); if(!slash) return strcmp(ip,cidr)==0;
    *slash=0; int prefix=atoi(slash+1);
    if (prefix<0||prefix>32) return 0;
    uint32_t ipv=ipr_parse(ip), netv=ipr_parse(net);
    uint32_t mask = prefix==0 ? 0 : (0xFFFFFFFFu << (32-prefix));
    return (ipv&mask)==(netv&mask);
}
int iprange_in_range(const char *ip, const char *low, const char *high) {
    if(!ip||!low||!high) return 0;
    uint32_t v=ipr_parse(ip), lo=ipr_parse(low), hi=ipr_parse(high);
    return (v>=lo && v<=hi)?1:0;
}
int iprange_is_private(const char *ip) {
    uint32_t v=ipr_parse(ip);
    if ((v&0xFF000000u)==0x0A000000u) return 1;
    /* 10.0.0.0/8 */
    if ((v&0xFFF00000u)==0xAC100000u) return 1;
    /* 172.16.0.0/12 */
    if ((v&0xFFFF0000u)==0xC0A80000u) return 1;
    /* 192.168.0.0/16 */
    if ((v&0xFF000000u)==0x7F000000u) return 1;
    /* 127.0.0.0/8 */
    return 0;
}
