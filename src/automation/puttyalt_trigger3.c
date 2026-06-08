/* puttyalt_trigger3.c - Trigger engine v3 with action chains + variables. */
#include <string.h>
#include <stdio.h>
#define TR_MAX 24
#define TR_PAT 96
#define TR_ACT 96
typedef struct { char pattern[TR_PAT]; char action[TR_ACT]; int action_type; int fire_count; int enabled; int once; } tr_trigger;
typedef struct { tr_trigger t[TR_MAX]; int n; } Trigger3;
enum tr_action { TR_NOTIFY=0, TR_LOG=1, TR_SEND=2, TR_HIGHLIGHT=3, TR_SOUND=4, TR_WEBHOOK=5 };
void trigger3_init(Trigger3 *t) { if(t) memset(t,0,sizeof(*t)); }
int trigger3_add(Trigger3 *t, const char *pattern, const char *action, int action_type, int once) {
    if(!t||t->n>=TR_MAX||!pattern) return -1;
    tr_trigger *tr=&t->t[t->n++]; snprintf(tr->pattern,TR_PAT,"%s",pattern); snprintf(tr->action,TR_ACT,"%s",action?action:"");
    tr->action_type=action_type; tr->enabled=1; tr->once=once?1:0; return t->n-1;
}
int trigger3_scan(Trigger3 *t, const char *line, int *out_actions, int cap) {
    if(!t||!line) return -1; int n=0;
    for (int i=0;i<t->n && n<cap;i++) {
        tr_trigger *tr=&t->t[i];
        if (!tr->enabled) continue;
        if (strstr(line,tr->pattern)) {
            tr->fire_count++; out_actions[n++]=i;
            if (tr->once) tr->enabled=0;
        }
    }
    return n;
}
const char *trigger3_action(const Trigger3 *t, int idx) {
    return (t&&idx>=0&&idx<t->n)?t->t[idx].action:0;
}
int trigger3_count(const Trigger3 *t) { return t?t->n:-1; }
