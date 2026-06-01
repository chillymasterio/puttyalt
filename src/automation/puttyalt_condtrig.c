/* puttyalt_condtrig.c - Conditional trigger: match -> action with cooldown. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CT_MAX 32
#define CT_PAT 96
typedef struct { char pattern[CT_PAT]; int action_id; int cooldown_ms; uint64_t last_fired; int fires; int enabled; } ct_rule;
typedef struct { ct_rule r[CT_MAX]; int n; } CondTrig;
void condtrig_init(CondTrig *c) { if(c) memset(c,0,sizeof(*c)); }
int condtrig_add(CondTrig *c, const char *pattern, int action_id, int cooldown_ms) {
    if(!c||c->n>=CT_MAX||!pattern) return -1;
    ct_rule *r=&c->r[c->n]; snprintf(r->pattern,CT_PAT,"%s",pattern); r->action_id=action_id;
    r->cooldown_ms=cooldown_ms; r->enabled=1; return c->n++;
}
int condtrig_feed(CondTrig *c, const char *line, uint64_t now_ms, int *out_actions, int cap) {
    if(!c||!line) return -1; int fired=0;
    for (int i=0;i<c->n;i++) {
        ct_rule *r=&c->r[i];
        if (!r->enabled) continue;
        if (strstr(line,r->pattern)) {
            if (r->last_fired && (now_ms-r->last_fired)<(uint64_t)r->cooldown_ms) continue;
            r->last_fired=now_ms; r->fires++;
            if (out_actions && fired<cap) out_actions[fired]=r->action_id;
            fired++;
        }
    }
    return fired;
}
void condtrig_enable(CondTrig *c, int idx, int on) { if(c&&idx>=0&&idx<c->n) c->r[idx].enabled=on?1:0; }
int condtrig_count(const CondTrig *c) { return c?c->n:-1; }
