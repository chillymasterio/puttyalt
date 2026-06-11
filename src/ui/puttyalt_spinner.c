/* puttyalt_spinner.c - Loading spinner animation (frame cycling). */
#include <stdint.h>
static const char *SPIN_DOTS[]={"\xe2\xa0\x8b","\xe2\xa0\x99","\xe2\xa0\xb9","\xe2\xa0\xb8","\xe2\xa0\xbc","\xe2\xa0\xb4","\xe2\xa0\xa6","\xe2\xa0\xa7","\xe2\xa0\x87","\xe2\xa0\x8f"};
static const char *SPIN_LINE[]={"|","/","-","\\"};
typedef struct { int frame; int style; uint64_t last_ms; int interval_ms; } Spinner;
void spinner_init(Spinner *s, int style, int interval_ms) {
    if(!s) return;
    s->frame=0; s->style=style; s->last_ms=0; s->interval_ms=interval_ms>0?interval_ms:80;
}
const char *spinner_tick(Spinner *s, uint64_t now_ms) {
    if(!s) return "";
    if (now_ms-s->last_ms>=(uint64_t)s->interval_ms) {
        int max = s->style==0 ? 10 : 4;
        s->frame=(s->frame+1)%max; s->last_ms=now_ms;
    }
    return s->style==0 ? SPIN_DOTS[s->frame] : SPIN_LINE[s->frame];
}
const char *spinner_current(const Spinner *s) {
    if(!s) return "";
    return s->style==0 ? SPIN_DOTS[s->frame] : SPIN_LINE[s->frame];
}
