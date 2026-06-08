/* puttyalt_macroplay.c - Macro playback engine with timing + loop. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MP_MAX 128
#define MP_DATA 64
typedef struct { char data[MP_DATA]; int delay_ms; } mp_step;
typedef struct { mp_step steps[MP_MAX]; int n; int cursor; int loop; int playing; uint64_t next_at; } MacroPlay;
void macroplay_init(MacroPlay *m) { if(m) memset(m,0,sizeof(*m)); }
int macroplay_record_step(MacroPlay *m, const char *data, int delay_ms) {
    if(!m||m->n>=MP_MAX||!data) return -1;
    mp_step *s=&m->steps[m->n++]; snprintf(s->data,MP_DATA,"%s",data); s->delay_ms=delay_ms; return 0;
}
int macroplay_start(MacroPlay *m, int loop, uint64_t now_ms) {
    if(!m||m->n==0) return -1; m->cursor=0; m->loop=loop?1:0; m->playing=1; m->next_at=now_ms; return 0;
}
int macroplay_tick(MacroPlay *m, uint64_t now_ms, char *out, int outlen) {
    if(!m||!m->playing||m->cursor>=m->n) return -1;
    if (now_ms < m->next_at) return 0; /* waiting */
    mp_step *s=&m->steps[m->cursor];
    if (out) snprintf(out,outlen,"%s",s->data);
    m->cursor++;
    if (m->cursor>=m->n) {
        if (m->loop) { m->cursor=0; m->next_at=now_ms+s->delay_ms; }
        else m->playing=0;
    } else m->next_at=now_ms+m->steps[m->cursor].delay_ms;
    return 1; /* step emitted */
}
void macroplay_stop(MacroPlay *m) { if(m) m->playing=0; }
int macroplay_is_playing(const MacroPlay *m) { return m?m->playing:-1; }
int macroplay_step_count(const MacroPlay *m) { return m?m->n:-1; }
