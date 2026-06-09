/* puttyalt_scrollback2.c - Scrollback ring buffer v2 with compression markers. */
#include <string.h>
#define SB_LINES 1024
typedef struct { int line_offsets[SB_LINES]; int head, count; int total_lines; int view_offset; } Scrollback2;
void scrollback2_init(Scrollback2 *s) { if(s) memset(s,0,sizeof(*s)); }
void scrollback2_push(Scrollback2 *s, int byte_offset) {
    if(!s) return;
    int idx=(s->head+s->count)%SB_LINES;
    if (s->count<SB_LINES) s->count++; else s->head=(s->head+1)%SB_LINES;
    s->line_offsets[idx]=byte_offset; s->total_lines++;
}
int scrollback2_get_offset(const Scrollback2 *s, int line_from_top) {
    if(!s||line_from_top<0||line_from_top>=s->count) return -1;
    return s->line_offsets[(s->head+line_from_top)%SB_LINES];
}
int scrollback2_scroll(Scrollback2 *s, int delta) {
    if(!s) return -1;
    s->view_offset+=delta;
    if (s->view_offset<0) s->view_offset=0;
    if (s->view_offset>s->count) s->view_offset=s->count;
    return s->view_offset;
}
int scrollback2_at_bottom(const Scrollback2 *s) { return (s&&s->view_offset==0)?1:0; }
void scrollback2_to_bottom(Scrollback2 *s) { if(s) s->view_offset=0; }
int scrollback2_visible_count(const Scrollback2 *s) { return s?s->count:-1; }
