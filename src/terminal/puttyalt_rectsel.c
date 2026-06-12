/* puttyalt_rectsel.c - Rectangular block selection model. */
#include <string.h>
typedef struct { int start_row, start_col, end_row, end_col; int active; int block_mode; } RectSel;
void rectsel_init(RectSel *r) { if(r) memset(r,0,sizeof(*r)); }
void rectsel_begin(RectSel *r, int row, int col, int block_mode) {
    if(!r) return;
    r->start_row=row; r->start_col=col; r->end_row=row; r->end_col=col; r->active=1; r->block_mode=block_mode?1:0;
}
void rectsel_extend(RectSel *r, int row, int col) { if(r&&r->active){ r->end_row=row; r->end_col=col; } }
void rectsel_end(RectSel *r) { if(r) r->active=0; }
int rectsel_contains(const RectSel *r, int row, int col) {
    if(!r) return 0;
    int r0=r->start_row<r->end_row?r->start_row:r->end_row;
    int r1=r->start_row<r->end_row?r->end_row:r->start_row;
    if (row<r0 || row>r1) return 0;
    if (r->block_mode) {
        int c0=r->start_col<r->end_col?r->start_col:r->end_col;
        int c1=r->start_col<r->end_col?r->end_col:r->start_col;
        return (col>=c0 && col<=c1);
    }
    /* linear selection */
    if (row==r0 && row==r1) { int c0=r->start_col<r->end_col?r->start_col:r->end_col, c1=r->start_col<r->end_col?r->end_col:r->start_col; return col>=c0&&col<=c1; }
    if (row==r0) return col>=(r->start_row<r->end_row?r->start_col:r->end_col);
    if (row==r1) return col<=(r->start_row<r->end_row?r->end_col:r->start_col);
    return 1;
}
int rectsel_is_block(const RectSel *r) { return r?r->block_mode:-1; }
int rectsel_is_active(const RectSel *r) { return r?r->active:-1; }
