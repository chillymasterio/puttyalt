/* puttyalt_underline.c - Extended underline styles (curly/dotted/dashed + color). */
#include <stdint.h>
enum ul_style { UL_NONE=0, UL_SINGLE=1, UL_DOUBLE=2, UL_CURLY=3, UL_DOTTED=4, UL_DASHED=5 };
typedef struct { int style; uint32_t color; int has_color; } Underline;
void underline_init(Underline *u) { if(u){ u->style=UL_NONE; u->color=0; u->has_color=0; } }
int underline_parse_sgr(Underline *u, int sgr_param, int sub_param) {
    if(!u) return -1;
    if (sgr_param==4) {
        switch(sub_param) {
            case 0: u->style=UL_NONE; break; case 1: u->style=UL_SINGLE; break;
            case 2: u->style=UL_DOUBLE; break; case 3: u->style=UL_CURLY; break;
            case 4: u->style=UL_DOTTED; break; case 5: u->style=UL_DASHED; break;
            default: u->style=UL_SINGLE; break;
        }
        return 0;
    }
    if (sgr_param==24) { u->style=UL_NONE; return 0; }
    return -1;
}
int underline_set_color(Underline *u, uint32_t color) { if(!u) return -1; u->color=color; u->has_color=1; return 0; }
void underline_reset_color(Underline *u) { if(u){ u->has_color=0; u->color=0; } }
int underline_is_active(const Underline *u) { return (u&&u->style!=UL_NONE)?1:0; }
int underline_style(const Underline *u) { return u?u->style:-1; }
