/* puttyalt_splitlayout.c - Named split layout presets (grid, columns, main+stack). */
#include <string.h>
#include <stdio.h>
enum sl_kind { SL_SINGLE=0, SL_COLUMNS=1, SL_GRID=2, SL_MAIN_STACK=3, SL_ROWS=4 };
typedef struct { int x,y,w,h; } sl_rect;
int splitlayout_compute(int kind, int n, int total_w, int total_h, sl_rect *out, int cap) {
    if(!out||n<=0||n>cap) return -1;
    if (kind==SL_SINGLE || n==1) { out[0].x=0; out[0].y=0; out[0].w=total_w; out[0].h=total_h; return 1; }
    if (kind==SL_COLUMNS) {
        int cw=total_w/n;
        for (int i=0;i<n;i++) { out[i].x=i*cw; out[i].y=0; out[i].w=(i==n-1)?total_w-i*cw:cw; out[i].h=total_h; }
        return n;
    }
    if (kind==SL_ROWS) {
        int rh=total_h/n;
        for (int i=0;i<n;i++) { out[i].x=0; out[i].y=i*rh; out[i].w=total_w; out[i].h=(i==n-1)?total_h-i*rh:rh; }
        return n;
    }
    if (kind==SL_MAIN_STACK) {
        int mw=total_w*6/10; out[0].x=0; out[0].y=0; out[0].w=mw; out[0].h=total_h;
        int sh=total_h/(n-1);
        for (int i=1;i<n;i++) { out[i].x=mw; out[i].y=(i-1)*sh; out[i].w=total_w-mw; out[i].h=(i==n-1)?total_h-(i-1)*sh:sh; }
        return n;
    }
    /* grid */
    int cols=1; while (cols*cols<n) cols++;
    int rows=(n+cols-1)/cols, cw=total_w/cols, rh=total_h/rows;
    for (int i=0;i<n;i++) { int r=i/cols,c=i%cols; out[i].x=c*cw; out[i].y=r*rh; out[i].w=cw; out[i].h=rh; }
    return n;
}
const char *splitlayout_name(int kind) {
    static const char *n[]={"single","columns","grid","main-stack","rows"};
    return (kind>=0&&kind<=SL_ROWS)?n[kind]:"unknown";
}
