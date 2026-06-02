/* puttyalt_cmdbar.c - Command bar input model with history navigation. */
#include <string.h>
#include <stdio.h>
#define CB_HIST 64
#define CB_LINE 256
typedef struct { char buf[CB_LINE]; int cursor; char hist[CB_HIST][CB_LINE]; int nhist; int hist_pos; } CmdBar;
void cmdbar_init(CmdBar *c) { if(c){ memset(c,0,sizeof(*c)); c->hist_pos=-1; } }
void cmdbar_insert(CmdBar *c, char ch) {
    if(!c||c->cursor>=CB_LINE-1) return;
    int len=(int)strlen(c->buf);
    memmove(&c->buf[c->cursor+1],&c->buf[c->cursor],len-c->cursor+1);
    c->buf[c->cursor++]=ch;
}
void cmdbar_backspace(CmdBar *c) {
    if(!c||c->cursor==0) return; int len=(int)strlen(c->buf);
    memmove(&c->buf[c->cursor-1],&c->buf[c->cursor],len-c->cursor+1); c->cursor--;
}
int cmdbar_submit(CmdBar *c, char *out, int outlen) {
    if(!c||!c->buf[0]) return -1;
    if (out) snprintf(out,outlen,"%s",c->buf);
    if (c->nhist>=CB_HIST) { memmove(c->hist[0],c->hist[1],sizeof(c->hist[0])*(CB_HIST-1)); c->nhist--; }
    snprintf(c->hist[c->nhist++],CB_LINE,"%s",c->buf);
    c->buf[0]=0; c->cursor=0; c->hist_pos=-1; return 0;
}
int cmdbar_history_prev(CmdBar *c) {
    if(!c||c->nhist==0) return -1;
    if (c->hist_pos<0) c->hist_pos=c->nhist-1; else if (c->hist_pos>0) c->hist_pos--;
    snprintf(c->buf,CB_LINE,"%s",c->hist[c->hist_pos]); c->cursor=(int)strlen(c->buf); return 0;
}
int cmdbar_history_next(CmdBar *c) {
    if(!c||c->hist_pos<0) return -1;
    if (c->hist_pos<c->nhist-1) { c->hist_pos++; snprintf(c->buf,CB_LINE,"%s",c->hist[c->hist_pos]); }
    else { c->hist_pos=-1; c->buf[0]=0; }
    c->cursor=(int)strlen(c->buf); return 0;
}
const char *cmdbar_text(const CmdBar *c) { return c?c->buf:""; }
