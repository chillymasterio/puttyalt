/* puttyalt_dashboard2.c - Dashboard v2: widget grid with live metric bindings. */
#include <string.h>
#include <stdio.h>
#define DB_MAX 24
#define DB_NAME 40
enum db_widget { DB_GRAPH=0, DB_GAUGE=1, DB_TABLE=2, DB_SPARK=3, DB_TEXT=4 };
typedef struct { char title[DB_NAME]; int type; int col, row, w, h; char metric[DB_NAME]; int visible; } db_widget;
typedef struct { db_widget w[DB_MAX]; int n; int cols, rows; } Dashboard2;
void dashboard2_init(Dashboard2 *d, int cols, int rows) {
    if(!d) return;
    memset(d,0,sizeof(*d)); d->cols=cols>0?cols:4; d->rows=rows>0?rows:3;
}
int dashboard2_add(Dashboard2 *d, const char *title, int type, int col, int row, int w, int h, const char *metric) {
    if(!d||d->n>=DB_MAX) return -1;
    if (col+w>d->cols || row+h>d->rows) return -1;
    /* out of grid */
    db_widget *wg=&d->w[d->n]; snprintf(wg->title,DB_NAME,"%s",title?title:""); wg->type=type;
    wg->col=col; wg->row=row; wg->w=w; wg->h=h; snprintf(wg->metric,DB_NAME,"%s",metric?metric:""); wg->visible=1;
    return d->n++;
}
int dashboard2_at(const Dashboard2 *d, int col, int row) {
    if(!d) return -1;
    for (int i=0;i<d->n;i++) {
        db_widget *w=&d->w[i];
        if (w->visible && col>=w->col && col<w->col+w->w && row>=w->row && row<w->row+w->h) return i;
    }
    return -1;
}
int dashboard2_collides(const Dashboard2 *d, int col, int row, int w, int h) {
    if(!d) return 0;
    for (int i=0;i<d->n;i++) {
        db_widget *g=&d->w[i];
        if (!(col+w<=g->col || g->col+g->w<=col || row+h<=g->row || g->row+g->h<=row)) return 1;
    }
    return 0;
}
int dashboard2_count(const Dashboard2 *d) { return d?d->n:-1; }
