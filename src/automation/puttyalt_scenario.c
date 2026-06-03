/* puttyalt_scenario.c - Scenario runner: scripted expect/send steps with timeouts. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SC_MAX 24
#define SC_TEXT 160
enum sc_kind { SC_EXPECT=0, SC_SEND=1, SC_WAIT=2 };
enum sc_state { SC_IDLE=0, SC_WAITING, SC_MATCHED, SC_TIMEDOUT, SC_COMPLETE };
typedef struct { int kind; char text[SC_TEXT]; int timeout_ms; } sc_step;
typedef struct { sc_step steps[SC_MAX]; int n; int cursor; int state; uint64_t step_start; } Scenario;
void scenario_init(Scenario *s) { if(s){ memset(s,0,sizeof(*s)); s->state=SC_IDLE; } }
int scenario_add(Scenario *s, int kind, const char *text, int timeout_ms) {
    if(!s||s->n>=SC_MAX) return -1;
    sc_step *st=&s->steps[s->n]; st->kind=kind; snprintf(st->text,SC_TEXT,"%s",text?text:""); st->timeout_ms=timeout_ms>0?timeout_ms:5000;
    return s->n++;
}
int scenario_start(Scenario *s, uint64_t now_ms) {
    if(!s||s->n==0) return -1; s->cursor=0; s->state=SC_WAITING; s->step_start=now_ms; return 0;
}
int scenario_feed(Scenario *s, const char *output, uint64_t now_ms) {
    if(!s||s->state!=SC_WAITING||s->cursor>=s->n) return -1;
    sc_step *st=&s->steps[s->cursor];
    if ((now_ms-s->step_start)>(uint64_t)st->timeout_ms) { s->state=SC_TIMEDOUT; return -1; }
    int advance=0;
    if (st->kind==SC_EXPECT) { if (output && strstr(output,st->text)) advance=1; }
    else advance=1; /* SEND/WAIT auto-advance */
    if (advance) {
        s->cursor++;
        if (s->cursor>=s->n) { s->state=SC_COMPLETE; return 1; }
        s->step_start=now_ms;
        return 2; /* advanced, more steps */
    }
    return 0;
}
int scenario_state(const Scenario *s) { return s?s->state:-1; }
int scenario_current_kind(const Scenario *s) { return (s&&s->cursor<s->n)?s->steps[s->cursor].kind:-1; }
