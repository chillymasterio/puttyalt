/* puttyalt_auditquery.c - Audit log query engine (filter by user/time/action). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define AQ_MAX 256
#define AQ_USER 32
#define AQ_ACTION 48
typedef struct { char user[AQ_USER]; char action[AQ_ACTION]; uint64_t ts; int severity; } aq_event;
typedef struct { aq_event e[AQ_MAX]; int n; } AuditQuery;
void auditquery_init(AuditQuery *a) { if(a) memset(a,0,sizeof(*a)); }
int auditquery_log(AuditQuery *a, const char *user, const char *action, int severity, uint64_t ts) {
    if(!a||!user||!action) return -1;
    if (a->n>=AQ_MAX) { memmove(&a->e[0],&a->e[1],sizeof(aq_event)*(AQ_MAX-1)); a->n--; }
    aq_event *e=&a->e[a->n++]; snprintf(e->user,AQ_USER,"%s",user); snprintf(e->action,AQ_ACTION,"%s",action);
    e->severity=severity; e->ts=ts; return 0;
}
int auditquery_by_user(const AuditQuery *a, const char *user, int *out_idx, int cap) {
    if(!a||!user) return -1;
    int n=0;
    for (int i=0;i<a->n && n<cap;i++) if (strcmp(a->e[i].user,user)==0) out_idx[n++]=i;
    return n;
}
int auditquery_by_timerange(const AuditQuery *a, uint64_t from, uint64_t to, int *out_idx, int cap) {
    if(!a) return -1;
    int n=0;
    for (int i=0;i<a->n && n<cap;i++) if (a->e[i].ts>=from && a->e[i].ts<=to) out_idx[n++]=i;
    return n;
}
int auditquery_high_severity(const AuditQuery *a, int min_sev, int *out_idx, int cap) {
    if(!a) return -1;
    int n=0;
    for (int i=0;i<a->n && n<cap;i++) if (a->e[i].severity>=min_sev) out_idx[n++]=i;
    return n;
}
int auditquery_count(const AuditQuery *a) { return a?a->n:-1; }
