/* puttyalt_reflow2.c - Text reflow on resize (rewrap long lines). */
#include <string.h>
typedef struct { int old_cols; int new_cols; int lines_before; int lines_after; } Reflow2;
void reflow2_init(Reflow2 *r) { if(r) memset(r,0,sizeof(*r)); }
/* Compute how many display lines a logical line of `length` chars takes at `cols`. */
int reflow2_display_lines(int length, int cols) {
    if (cols<=0) return 1;
    if (length==0) return 1;
    return (length + cols - 1) / cols;
}
int reflow2_resize(Reflow2 *r, int old_cols, int new_cols, const int *line_lengths, int n) {
    if(!r||!line_lengths) return -1;
    r->old_cols=old_cols; r->new_cols=new_cols; r->lines_before=0; r->lines_after=0;
    for (int i=0;i<n;i++) {
        r->lines_before += reflow2_display_lines(line_lengths[i],old_cols);
        r->lines_after += reflow2_display_lines(line_lengths[i],new_cols);
    }
    return r->lines_after;
}
int reflow2_cursor_adjust(const Reflow2 *r, int cursor_line, int cursor_col, const int *line_lengths, int *new_line) {
    if(!r||!line_lengths||!new_line) return -1;
    int abs_chars=0;
    for (int i=0;i<cursor_line;i++) abs_chars += line_lengths[i];
    abs_chars += cursor_col;
    *new_line = r->new_cols>0 ? abs_chars / r->new_cols : 0;
    return r->new_cols>0 ? abs_chars % r->new_cols : 0;
}
