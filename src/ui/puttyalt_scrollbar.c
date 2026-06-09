/* puttyalt_scrollbar.c - Scrollbar geometry + thumb hit testing. */
#include <string.h>
typedef struct { int total; int visible; int position; int track_height; int thumb_y; int thumb_h; int dragging; } Scrollbar;
void scrollbar_init(Scrollbar *s, int track_height) {
    if(!s) return; memset(s,0,sizeof(*s)); s->track_height=track_height;
}
void scrollbar_update(Scrollbar *s, int total, int visible, int position) {
    if(!s) return; s->total=total; s->visible=visible; s->position=position;
    if (total<=visible) { s->thumb_h=s->track_height; s->thumb_y=0; return; }
    s->thumb_h = s->visible * s->track_height / s->total;
    if (s->thumb_h<16) s->thumb_h=16;
    int max_pos = total - visible;
    s->thumb_y = max_pos>0 ? position * (s->track_height - s->thumb_h) / max_pos : 0;
}
int scrollbar_hit_thumb(const Scrollbar *s, int y) {
    if(!s) return 0; return (y>=s->thumb_y && y<s->thumb_y+s->thumb_h)?1:0;
}
int scrollbar_pos_from_y(const Scrollbar *s, int y) {
    if(!s||s->track_height<=s->thumb_h) return 0;
    int max_pos=s->total-s->visible; if(max_pos<0) max_pos=0;
    int p = y * max_pos / (s->track_height - s->thumb_h);
    if (p<0) p=0; if (p>max_pos) p=max_pos;
    return p;
}
int scrollbar_needed(const Scrollbar *s) { return (s&&s->total>s->visible)?1:0; }
