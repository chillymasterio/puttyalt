/* puttyalt_sesspin.c - Session pinning with priority order. */
#include <string.h>
#include <stdio.h>
#define SP_MAX 32
#define SP_NAME 64
typedef struct { char name[SP_NAME]; int pin_order; int pinned; } sp_session;
typedef struct { sp_session sessions[SP_MAX]; int n; int next_order; } SessPin;
void sesspin_init(SessPin *s) { if(s) memset(s,0,sizeof(*s)); }
int sesspin_register(SessPin *s, const char *name) {
    if(!s||s->n>=SP_MAX||!name) return -1;
    snprintf(s->sessions[s->n].name,SP_NAME,"%s",name); return s->n++;
}
int sesspin_pin(SessPin *s, const char *name) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->sessions[i].name,name)==0) { s->sessions[i].pinned=1; s->sessions[i].pin_order=s->next_order++; return 0; }
    return -1;
}
int sesspin_unpin(SessPin *s, const char *name) {
    if(!s||!name) return -1;
    for (int i=0;i<s->n;i++) if (strcmp(s->sessions[i].name,name)==0) { s->sessions[i].pinned=0; return 0; }
    return -1;
}
int sesspin_ordered(const SessPin *s, int *out_idx, int cap) {
    if(!s) return -1; int n=0, used[SP_MAX]={0};
    /* pinned first by pin_order, then unpinned by registration */
    for (int k=0;k<cap;k++) {
        int best=-1;
        for (int i=0;i<s->n;i++) if (!used[i] && s->sessions[i].pinned) { if(best<0||s->sessions[i].pin_order<s->sessions[best].pin_order) best=i; }
        if (best<0) for (int i=0;i<s->n;i++) if (!used[i]) { best=i; break; }
        if (best<0) break; used[best]=1; out_idx[n++]=best;
    }
    return n;
}
int sesspin_count(const SessPin *s) { return s?s->n:-1; }
