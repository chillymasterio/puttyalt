/* puttyalt_wrapindent.c - Wrapped-line continuation indent tracking. */
#include <string.h>
typedef struct { int indent_width; int wrap_marker; int cols; } WrapIndent;
void wrapindent_init(WrapIndent *w, int cols, int indent_width) {
    if(!w) return;
    w->cols=cols>0?cols:80; w->indent_width=indent_width>=0?indent_width:2; w->wrap_marker=1;
}
int wrapindent_visual_lines(const WrapIndent *w, int text_len) {
    if(!w||w->cols<=0) return 1;
    if (text_len<=w->cols) return 1;
    int first=w->cols;
    int remaining=text_len-first;
    int per_line=w->cols-w->indent_width; if(per_line<1)per_line=1;
    return 1 + (remaining+per_line-1)/per_line;
}
int wrapindent_col_for(const WrapIndent *w, int char_index) {
    if(!w||w->cols<=0) return 0;
    if (char_index<w->cols) return char_index;
    int rem=char_index-w->cols;
    int per_line=w->cols-w->indent_width; if(per_line<1)per_line=1;
    return w->indent_width + (rem%per_line);
}
int wrapindent_row_for(const WrapIndent *w, int char_index) {
    if(!w||w->cols<=0) return 0;
    if (char_index<w->cols) return 0;
    int rem=char_index-w->cols;
    int per_line=w->cols-w->indent_width; if(per_line<1)per_line=1;
    return 1 + rem/per_line;
}
