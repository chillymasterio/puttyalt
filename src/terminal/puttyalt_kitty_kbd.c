/* puttyalt_kitty_kbd.c - Kitty keyboard protocol (CSI u) encoder/decoder. */
#include <string.h>
#include <stdio.h>
#define KK_SHIFT 1
#define KK_ALT 2
#define KK_CTRL 4
#define KK_SUPER 8
typedef struct { int enabled; int flags; } KittyKbd;
void kitty_kbd_init(KittyKbd *k) { if(k){ k->enabled=0; k->flags=0; } }
void kitty_kbd_set_enabled(KittyKbd *k, int flags) { if(k){ k->enabled=flags?1:0; k->flags=flags; } }
int kitty_kbd_encode(const KittyKbd *k, unsigned int codepoint, int mods, char *out, int outlen) {
    if(!k||!out) return -1;
    if (!k->enabled) {
        if (codepoint<128) { out[0]=(char)codepoint; out[1]=0; return 1; }
        return 0;
    }
    int mod_param = mods+1; /* kitty modifiers are 1-based */
    if (mod_param>1) return snprintf(out,outlen,"\x1b[%u;%du",codepoint,mod_param);
    return snprintf(out,outlen,"\x1b[%uu",codepoint);
}
int kitty_kbd_decode(const char *seq, int len, unsigned int *codepoint, int *mods) {
    if(!seq||len<3) return -1;
    if (seq[0]!='\x1b'||seq[1]!='[') return -1;
    unsigned int cp=0; int m=0;
    if (sscanf(seq+2,"%u;%d",&cp,&m)>=1) { if(codepoint)*codepoint=cp; if(mods)*mods=m>0?m-1:0; return 0; }
    return -1;
}
int kitty_kbd_is_enabled(const KittyKbd *k) { return k?k->enabled:-1; }
