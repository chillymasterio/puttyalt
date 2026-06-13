/* puttyalt_zoompan.c - Zoom/pan transform for image preview panes. */
#include <string.h>
typedef struct { float zoom; float pan_x, pan_y; float min_z, max_z; int vw, vh; } ZoomPan;
void zoompan_init(ZoomPan *z, int vw, int vh) {
    if(!z) return;
    memset(z,0,sizeof(*z)); z->zoom=1.0f; z->min_z=0.1f; z->max_z=10.0f; z->vw=vw; z->vh=vh;
}
void zoompan_zoom_at(ZoomPan *z, float factor, int cx, int cy) {
    if(!z) return;
    float nz=z->zoom*factor;
    if (nz<z->min_z) nz=z->min_z;
    if (nz>z->max_z) nz=z->max_z;
    z->pan_x = cx - (cx - z->pan_x)*(nz/z->zoom);
    z->pan_y = cy - (cy - z->pan_y)*(nz/z->zoom);
    z->zoom=nz;
}
void zoompan_pan(ZoomPan *z, float dx, float dy) { if(z){ z->pan_x+=dx; z->pan_y+=dy; } }
void zoompan_reset(ZoomPan *z) { if(z){ z->zoom=1.0f; z->pan_x=z->pan_y=0; } }
float zoompan_level(const ZoomPan *z) { return z?z->zoom:0; }
