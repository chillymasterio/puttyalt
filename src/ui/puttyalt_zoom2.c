/* puttyalt_zoom2.c - UI zoom/scaling with font size steps + DPI. */
#include <string.h>
typedef struct { int base_font; int current_font; int min_font; int max_font; int dpi_scale; } Zoom2;
void zoom2_init(Zoom2 *z, int base_font, int dpi_scale) {
    if(!z) return; memset(z,0,sizeof(*z)); z->base_font=base_font>0?base_font:14; z->current_font=z->base_font;
    z->min_font=8; z->max_font=72; z->dpi_scale=dpi_scale>0?dpi_scale:100;
}
int zoom2_in(Zoom2 *z) {
    if(!z) return -1; if(z->current_font<z->max_font) z->current_font++; return z->current_font;
}
int zoom2_out(Zoom2 *z) {
    if(!z) return -1; if(z->current_font>z->min_font) z->current_font--; return z->current_font;
}
void zoom2_reset(Zoom2 *z) { if(z) z->current_font=z->base_font; }
int zoom2_effective_size(const Zoom2 *z) {
    if(!z) return 0; return z->current_font * z->dpi_scale / 100;
}
int zoom2_percent(const Zoom2 *z) {
    if(!z||z->base_font==0) return 100; return z->current_font*100/z->base_font;
}
