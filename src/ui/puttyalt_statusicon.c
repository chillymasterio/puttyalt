/* puttyalt_statusicon.c - Connection status icon state machine (idle/connecting/up/err). */
#include <string.h>
enum si_state { SI_IDLE=0, SI_CONNECTING=1, SI_CONNECTED=2, SI_RECONNECTING=3, SI_ERROR=4, SI_DISCONNECTED=5 };
typedef struct { int state; int blink_phase; int latency_ms; int signal_bars; } StatusIcon;
void statusicon_init(StatusIcon *s) { if(s) memset(s,0,sizeof(*s)); }
void statusicon_set_state(StatusIcon *s, int state) { if(s){ s->state=state; s->blink_phase=0; } }
void statusicon_update_latency(StatusIcon *s, int latency_ms) {
    if(!s) return; s->latency_ms=latency_ms;
    if (latency_ms<50) s->signal_bars=4; else if (latency_ms<150) s->signal_bars=3;
    else if (latency_ms<400) s->signal_bars=2; else s->signal_bars=1;
}
int statusicon_should_blink(const StatusIcon *s) {
    if(!s) return 0; return (s->state==SI_CONNECTING||s->state==SI_RECONNECTING)?1:0;
}
void statusicon_tick(StatusIcon *s) { if(s&&statusicon_should_blink(s)) s->blink_phase^=1; }
int statusicon_glyph(const StatusIcon *s) {
    if(!s) return 0;
    switch(s->state) { case SI_CONNECTED: return s->signal_bars; case SI_ERROR: return -1; default: return 0; }
}
int statusicon_state(const StatusIcon *s) { return s?s->state:-1; }
