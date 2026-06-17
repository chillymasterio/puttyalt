/* puttyalt_diffview.c - Side-by-side diff view layout. */
#include <string.h>
#define DV_MAX 256
enum dv_op { DV_SAME=0, DV_ADD=1, DV_DEL=2, DV_MOD=3 };
typedef struct { int left_line; int right_line; int op; } dv_row;
typedef struct { dv_row rows[DV_MAX]; int n; int additions; int deletions; } DiffView;
void diffview_init(DiffView *d) { if(d) memset(d,0,sizeof(*d)); }
int diffview_add_row(DiffView *d, int left, int right, int op) {
    if(!d||d->n>=DV_MAX) return -1;
    dv_row *r=&d->rows[d->n++]; r->left_line=left; r->right_line=right; r->op=op;
    if (op==DV_ADD) d->additions++; else if (op==DV_DEL) d->deletions++;
    return d->n-1;
}
int diffview_row_op(const DiffView *d, int idx) { return (d&&idx>=0&&idx<d->n)?d->rows[idx].op:-1; }
int diffview_changed_count(const DiffView *d) { return d?(d->additions+d->deletions):-1; }
int diffview_summary(const DiffView *d, int *adds, int *dels) {
    if(!d) return -1;
    if(adds) *adds=d->additions;
    if(dels)*dels=d->deletions;
    return d->n;
}
