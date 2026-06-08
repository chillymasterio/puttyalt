/* puttyalt_statusline2.c - Status line v2 with powerline segments. */
#include <string.h>
#include <stdio.h>
#define SL_MAX 12
#define SL_TEXT 48
typedef struct { char text[SL_TEXT]; unsigned int fg, bg; int align; } sl_seg;
typedef struct { sl_seg segs[SL_MAX]; int n; int powerline; } StatusLine2;
void statusline2_init(StatusLine2 *s, int powerline) { if(s){ memset(s,0,sizeof(*s)); s->powerline=powerline?1:0; } }
int statusline2_add(StatusLine2 *s, const char *text, unsigned int fg, unsigned int bg, int align) {
    if(!s||s->n>=SL_MAX||!text) return -1;
    sl_seg *seg=&s->segs[s->n++]; snprintf(seg->text,SL_TEXT,"%s",text); seg->fg=fg; seg->bg=bg; seg->align=align;
    return 0;
}
int statusline2_render(const StatusLine2 *s, char *buf, int buflen) {
    if(!s||!buf) return -1; int pos=0;
    for (int i=0;i<s->n && pos<buflen;i++) {
        if (s->powerline && i>0) pos+=snprintf(buf+pos,buflen-pos,"\xee\x82\xb0"); /* powerline separator */
        pos+=snprintf(buf+pos,buflen-pos," %s ",s->segs[i].text);
    }
    return pos;
}
void statusline2_clear(StatusLine2 *s) { if(s) s->n=0; }
int statusline2_count(const StatusLine2 *s) { return s?s->n:-1; }
