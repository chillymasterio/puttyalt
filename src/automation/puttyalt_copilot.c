/* puttyalt_copilot.c - AI copilot session: turn history + suggestion ranking state. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define CP_TURNS 32
#define CP_TEXT 512
enum cp_role { CP_USER=0, CP_ASSISTANT=1, CP_SYSTEM=2 };
typedef struct { int role; char text[CP_TEXT]; uint64_t ts; } cp_turn;
typedef struct { cp_turn turns[CP_TURNS]; int n; int max_context; int total_tokens; } Copilot;
void copilot_init(Copilot *c, int max_context) {
    if(!c) return; memset(c,0,sizeof(*c)); c->max_context=max_context>0?max_context:8;
}
int copilot_add_turn(Copilot *c, int role, const char *text, uint64_t now_ms) {
    if(!c||!text) return -1;
    if (c->n>=CP_TURNS) { memmove(&c->turns[0],&c->turns[1],sizeof(cp_turn)*(CP_TURNS-1)); c->n--; }
    cp_turn *t=&c->turns[c->n++]; t->role=role; snprintf(t->text,CP_TEXT,"%s",text); t->ts=now_ms;
    c->total_tokens += (int)strlen(text)/4;
    return 0;
}
int copilot_build_context(const Copilot *c, char *out, int outlen) {
    if(!c||!out) return -1; int pos=0;
    int start = c->n - c->max_context; if (start<0) start=0;
    for (int i=start;i<c->n && pos<outlen;i++) {
        const char *r = c->turns[i].role==CP_USER?"user":(c->turns[i].role==CP_ASSISTANT?"assistant":"system");
        pos+=snprintf(out+pos,outlen-pos,"%s: %s\n",r,c->turns[i].text);
    }
    return pos;
}
void copilot_clear(Copilot *c) { if(c){ c->n=0; c->total_tokens=0; } }
int copilot_turn_count(const Copilot *c) { return c?c->n:-1; }
