/* puttyalt_alertrule.c - Alert rule engine: threshold + duration -> fire/clear. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define AR_MAX 24
#define AR_NAME 48
enum ar_state { AR_OK=0, AR_PENDING=1, AR_FIRING=2 };
typedef struct { char name[AR_NAME]; double threshold; int above; int for_ms; int state; uint64_t since_ms; } ar_rule;
typedef struct { ar_rule r[AR_MAX]; int n; } AlertRule;
void alertrule_init(AlertRule *a) { if(a) memset(a,0,sizeof(*a)); }
int alertrule_add(AlertRule *a, const char *name, double threshold, int above, int for_ms) {
    if(!a||a->n>=AR_MAX||!name) return -1;
    ar_rule *r=&a->r[a->n]; snprintf(r->name,AR_NAME,"%s",name); r->threshold=threshold; r->above=above?1:0; r->for_ms=for_ms;
    r->state=AR_OK; return a->n++;
}
int alertrule_eval(AlertRule *a, int idx, double value, uint64_t now_ms) {
    if(!a||idx<0||idx>=a->n) return -1;
    ar_rule *r=&a->r[idx];
    int breached = r->above ? (value>r->threshold) : (value<r->threshold);
    if (breached) {
        if (r->state==AR_OK) { r->state=AR_PENDING; r->since_ms=now_ms; }
        else if (r->state==AR_PENDING && (now_ms-r->since_ms)>=(uint64_t)r->for_ms) { r->state=AR_FIRING; return 1; }
    } else {
        int was_firing = (r->state==AR_FIRING);
        r->state=AR_OK; if (was_firing) return 2; /* cleared */
    }
    return 0;
}
int alertrule_state(const AlertRule *a, int idx) { return (a&&idx>=0&&idx<a->n)?a->r[idx].state:-1; }
int alertrule_firing_count(const AlertRule *a) {
    if(!a) return -1; int n=0; for(int i=0;i<a->n;i++) if(a->r[i].state==AR_FIRING)n++; return n;
}
