/* puttyalt_proto2.c - Protocol version negotiation (SSH banner exchange). */
#include <string.h>
#include <stdio.h>
typedef struct { char remote_version[128]; int proto_major; int proto_minor; char software[64]; int compatible; } Proto2;
void proto2_init(Proto2 *p) { if(p) memset(p,0,sizeof(*p)); }
int proto2_parse_banner(Proto2 *p, const char *banner) {
    if(!p||!banner) return -1;
    if (strncmp(banner,"SSH-",4)!=0) return -1;
    snprintf(p->remote_version,128,"%s",banner);
    if (sscanf(banner,"SSH-%d.%d-%63s",&p->proto_major,&p->proto_minor,p->software)>=2) {
        p->compatible = (p->proto_major==2) ? 1 : 0;
        return 0;
    }
    return -1;
}
int proto2_build_banner(int major, int minor, const char *software, char *out, int outlen) {
    return snprintf(out,outlen,"SSH-%d.%d-%s",major,minor,software?software:"PuttyAlt_3.8");
}
int proto2_is_compatible(const Proto2 *p) { return p?p->compatible:-1; }
const char *proto2_software(const Proto2 *p) { return p?p->software:""; }
