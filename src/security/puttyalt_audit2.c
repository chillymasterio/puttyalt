/* puttyalt_audit2.c - Audit log v2 with severity levels + retention. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define AU_MAX 256
#define AU_MSG 128
enum au_sev { AU_DEBUG=0, AU_INFO=1, AU_NOTICE=2, AU_WARN=3, AU_ERROR=4, AU_CRIT=5 };
typedef struct { uint64_t ts; int severity; char actor[32]; char message[AU_MSG]; } au_event;
typedef struct { au_event events[AU_MAX]; int head, count; int min_severity; } Audit2;
void audit2_init(Audit2 *a, int min_severity) {
    if(!a) return; memset(a,0,sizeof(*a)); a->min_severity=min_severity;
}
int audit2_log(Audit2 *a, int severity, const char *actor, const char *message, uint64_t ts) {
    if(!a||severity<a->min_severity) return 0;
    int idx=(a->head+a->count)%AU_MAX;
    if (a->count<AU_MAX) a->count++; else a->head=(a->head+1)%AU_MAX;
    au_event *e=&a->events[idx]; e->ts=ts; e->severity=severity;
    snprintf(e->actor,32,"%s",actor?actor:"system"); snprintf(e->message,AU_MSG,"%s",message?message:"");
    return 1;
}
int audit2_count_severity(const Audit2 *a, int severity) {
    if(!a) return -1; int n=0;
    for (int i=0;i<a->count;i++) { int idx=(a->head+i)%AU_MAX; if(a->events[idx].severity==severity)n++; }
    return n;
}
int audit2_export(const Audit2 *a, int min_sev, char *out, int outlen) {
    if(!a||!out) return -1; int pos=0;
    static const char *sn[]={"DEBUG","INFO","NOTICE","WARN","ERROR","CRIT"};
    for (int i=0;i<a->count && pos<outlen;i++) {
        int idx=(a->head+i)%AU_MAX; au_event *e=&a->events[idx];
        if (e->severity>=min_sev)
            pos+=snprintf(out+pos,outlen-pos,"[%s] %s: %s\n",sn[e->severity<6?e->severity:1],e->actor,e->message);
    }
    return pos;
}
int audit2_count(const Audit2 *a) { return a?a->count:-1; }
