/* puttyalt_oscfont.c - OSC font/cursor color query+set (OSC 10/11/12). */
#include <string.h>
#include <stdio.h>
typedef struct { unsigned int fg, bg, cursor; int fg_set, bg_set, cursor_set; } OscFont;
void oscfont_init(OscFont *o) { if(o) memset(o,0,sizeof(*o)); }
int oscfont_parse(OscFont *o, int osc_num, const char *value) {
    if(!o||!value) return -1;
    unsigned int color=0;
    /* rgb:RRRR/GGGG/BBBB or #RRGGBB */
    if (value[0]=='#') { if(sscanf(value+1,"%6x",&color)!=1) return -1; }
    else if (!strncmp(value,"rgb:",4)) {
        int r,g,b; if (sscanf(value+4,"%2x%*2x/%2x%*2x/%2x",&r,&g,&b)>=3 || sscanf(value+4,"%x/%x/%x",&r,&g,&b)==3) color=((r&0xFF)<<16)|((g&0xFF)<<8)|(b&0xFF); else return -1;
    } else if (strcmp(value,"?")==0) return 1; /* query */
    else return -1;
    switch(osc_num) {
        case 10: o->fg=color; o->fg_set=1; break;
        case 11: o->bg=color; o->bg_set=1; break;
        case 12: o->cursor=color; o->cursor_set=1; break;
        default: return -1;
    }
    return 0;
}
int oscfont_format_response(int osc_num, unsigned int color, char *out, int outlen) {
    int r=(color>>16)&0xFF, g=(color>>8)&0xFF, b=color&0xFF;
    return snprintf(out,outlen,"\x1b]%d;rgb:%02x%02x/%02x%02x/%02x%02x\x07",osc_num,r,r,g,g,b,b);
}
