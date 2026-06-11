/* puttyalt_schedrule.c - Schedule rule engine (interval/daily/weekly). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SR_MAX 24
enum sr_type { SR_INTERVAL=0, SR_DAILY=1, SR_WEEKLY=2, SR_ONCE=3 };
typedef struct { int type; int interval_sec; int hour, minute; int weekday; uint64_t next_run; int enabled; char action[64]; } sr_rule;
typedef struct { sr_rule rules[SR_MAX]; int n; } SchedRule;
void schedrule_init(SchedRule *s) { if(s) memset(s,0,sizeof(*s)); }
int schedrule_add_interval(SchedRule *s, const char *action, int interval_sec, uint64_t now_ms) {
    if(!s||s->n>=SR_MAX) return -1;
    sr_rule *r=&s->rules[s->n]; r->type=SR_INTERVAL; r->interval_sec=interval_sec;
    snprintf(r->action,64,"%s",action?action:""); r->next_run=now_ms+(uint64_t)interval_sec*1000; r->enabled=1;
    return s->n++;
}
int schedrule_add_daily(SchedRule *s, const char *action, int hour, int minute) {
    if(!s||s->n>=SR_MAX) return -1;
    sr_rule *r=&s->rules[s->n]; r->type=SR_DAILY; r->hour=hour; r->minute=minute;
    snprintf(r->action,64,"%s",action?action:""); r->enabled=1; return s->n++;
}
int schedrule_due(SchedRule *s, uint64_t now_ms, int *out_idx, int cap) {
    if(!s) return -1; int n=0;
    for (int i=0;i<s->n && n<cap;i++) {
        sr_rule *r=&s->rules[i];
        if (!r->enabled) continue;
        if (r->type==SR_INTERVAL && now_ms>=r->next_run) {
            out_idx[n++]=i; r->next_run=now_ms+(uint64_t)r->interval_sec*1000;
        }
    }
    return n;
}
const char *schedrule_action(const SchedRule *s, int idx) {
    return (s&&idx>=0&&idx<s->n)?s->rules[idx].action:0;
}
int schedrule_count(const SchedRule *s) { return s?s->n:-1; }
