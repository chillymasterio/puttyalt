/* puttyalt_compliance.c - Compliance policy framework (PCI/SOC2/HIPAA rule sets). */
#include <string.h>
#include <stdio.h>
#define CP_MAX 32
#define CP_NAME 48
enum cp_standard { CP_NONE=0, CP_PCI_DSS=1, CP_SOC2=2, CP_HIPAA=3, CP_GDPR=4 };
typedef struct { char rule[CP_NAME]; int standard; int required; int satisfied; } cp_rule;
typedef struct { cp_rule r[CP_MAX]; int n; int active_standard; } Compliance;
void compliance_init(Compliance *c, int standard) {
    if(!c) return; memset(c,0,sizeof(*c)); c->active_standard=standard;
}
int compliance_add_rule(Compliance *c, const char *rule, int standard, int required) {
    if(!c||c->n>=CP_MAX||!rule) return -1;
    cp_rule *r=&c->r[c->n++]; snprintf(r->rule,CP_NAME,"%s",rule); r->standard=standard; r->required=required?1:0;
    return 0;
}
void compliance_mark(Compliance *c, const char *rule, int satisfied) {
    if(!c||!rule) return;
    for (int i=0;i<c->n;i++) if (strcmp(c->r[i].rule,rule)==0) c->r[i].satisfied=satisfied?1:0;
}
int compliance_is_compliant(const Compliance *c) {
    if(!c) return 0;
    for (int i=0;i<c->n;i++)
        if (c->r[i].standard==c->active_standard && c->r[i].required && !c->r[i].satisfied) return 0;
    return 1;
}
int compliance_gaps(const Compliance *c, char *buf, int buflen) {
    if(!c||!buf) return -1; int pos=0, n=0;
    for (int i=0;i<c->n;i++)
        if (c->r[i].standard==c->active_standard && c->r[i].required && !c->r[i].satisfied && pos<buflen) {
            pos+=snprintf(buf+pos,buflen-pos,"%s\n",c->r[i].rule); n++;
        }
    return n;
}
