/* puttyalt_triggermatch.c - Multi-pattern trigger matcher (Aho-Corasick-lite). */
#include <string.h>
#include <stdio.h>
#define TM_MAX 32
#define TM_PAT 64
typedef struct { char pattern[TM_PAT]; int action_id; int matches; } tm_pattern;
typedef struct { tm_pattern patterns[TM_MAX]; int n; } TriggerMatch;
void triggermatch_init(TriggerMatch *t) { if(t) memset(t,0,sizeof(*t)); }
int triggermatch_add(TriggerMatch *t, const char *pattern, int action_id) {
    if(!t||t->n>=TM_MAX||!pattern) return -1;
    snprintf(t->patterns[t->n].pattern,TM_PAT,"%s",pattern); t->patterns[t->n].action_id=action_id; return t->n++;
}
int triggermatch_scan(TriggerMatch *t, const char *text, int *out_actions, int cap) {
    if(!t||!text) return -1;
    int n=0;
    for (int i=0;i<t->n && n<cap;i++) {
        if (strstr(text,t->patterns[i].pattern)) { t->patterns[i].matches++; out_actions[n++]=t->patterns[i].action_id; }
    }
    return n;
}
int triggermatch_match_count(const TriggerMatch *t, int idx) {
    return (t&&idx>=0&&idx<t->n)?t->patterns[idx].matches:-1;
}
int triggermatch_count(const TriggerMatch *t) { return t?t->n:-1; }
