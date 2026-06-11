/* puttyalt_selection2.c - Text selection v2 (char/word/line/block modes). */
#include <string.h>
enum sel_mode { SEL_CHAR=0, SEL_WORD=1, SEL_LINE=2, SEL_BLOCK=3 };
typedef struct { int anchor_row, anchor_col; int end_row, end_col; int mode; int active; } Selection2;
void selection2_init(Selection2 *s) { if(s) memset(s,0,sizeof(*s)); }
void selection2_begin(Selection2 *s, int row, int col, int mode) {
    if(!s) return; s->anchor_row=row; s->anchor_col=col; s->end_row=row; s->end_col=col; s->mode=mode; s->active=1;
}
void selection2_extend(Selection2 *s, int row, int col) { if(s&&s->active){ s->end_row=row; s->end_col=col; } }
int selection2_normalized(const Selection2 *s, int *r0, int *c0, int *r1, int *c1) {
    if(!s) return -1;
    int ar=s->anchor_row, ac=s->anchor_col, er=s->end_row, ec=s->end_col;
    if (ar>er || (ar==er && ac>ec)) { int t; t=ar;ar=er;er=t; t=ac;ac=ec;ec=t; }
    if(r0)*r0=ar; if(c0)*c0=ac; if(r1)*r1=er; if(c1)*c1=ec; return 0;
}
int selection2_contains(const Selection2 *s, int row, int col) {
    if(!s||!s->active) return 0;
    int r0,c0,r1,c1; selection2_normalized(s,&r0,&c0,&r1,&c1);
    if (s->mode==SEL_LINE) return row>=r0 && row<=r1;
    if (s->mode==SEL_BLOCK) { int cl=c0<c1?c0:c1, cr=c0<c1?c1:c0; return row>=r0&&row<=r1&&col>=cl&&col<=cr; }
    if (row<r0||row>r1) return 0;
    if (row==r0 && row==r1) return col>=c0&&col<=c1;
    if (row==r0) return col>=c0; if (row==r1) return col<=c1;
    return 1;
}
void selection2_clear(Selection2 *s) { if(s) s->active=0; }
int selection2_is_active(const Selection2 *s) { return s?s->active:-1; }
