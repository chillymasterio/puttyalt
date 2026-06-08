/* puttyalt_sessnote2.c - Session notes v2 with markdown + pinned items. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SN_MAX 32
#define SN_TEXT 256
typedef struct { char text[SN_TEXT]; uint64_t created; int pinned; } sn_note;
typedef struct { sn_note notes[SN_MAX]; int n; } SessNote2;
void sessnote2_init(SessNote2 *s) { if(s) memset(s,0,sizeof(*s)); }
int sessnote2_add(SessNote2 *s, const char *text, uint64_t now_ms) {
    if(!s||s->n>=SN_MAX||!text) return -1;
    sn_note *n=&s->notes[s->n++]; snprintf(n->text,SN_TEXT,"%s",text); n->created=now_ms; return s->n-1;
}
void sessnote2_pin(SessNote2 *s, int idx, int pin) { if(s&&idx>=0&&idx<s->n) s->notes[idx].pinned=pin?1:0; }
int sessnote2_ordered(const SessNote2 *s, int *out_idx, int cap) {
    if(!s) return -1; int n=0;
    /* pinned first */
    for (int i=0;i<s->n && n<cap;i++) if (s->notes[i].pinned) out_idx[n++]=i;
    for (int i=0;i<s->n && n<cap;i++) if (!s->notes[i].pinned) out_idx[n++]=i;
    return n;
}
int sessnote2_search(const SessNote2 *s, const char *query, int *out_idx, int cap) {
    if(!s||!query) return -1; int n=0;
    for (int i=0;i<s->n && n<cap;i++) if (strstr(s->notes[i].text,query)) out_idx[n++]=i;
    return n;
}
int sessnote2_count(const SessNote2 *s) { return s?s->n:-1; }
