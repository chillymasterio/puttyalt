/* puttyalt_touchgest.c - Touch gesture recognizer (tap, swipe, pinch, long-press). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define TG_MAX_POINTS 8
enum tg_gesture { TG_NONE=0, TG_TAP, TG_DOUBLETAP, TG_SWIPE_L, TG_SWIPE_R, TG_SWIPE_U, TG_SWIPE_D, TG_PINCH_IN, TG_PINCH_OUT, TG_LONGPRESS };
typedef struct { int id, x, y, sx, sy; uint64_t t0; int active; } tg_point;
typedef struct { tg_point pts[TG_MAX_POINTS]; int count; int swipe_thresh; int longpress_ms; uint64_t last_tap_ms; } TouchGesture;
void touchgest_init(TouchGesture *g, int swipe_thresh, int longpress_ms) {
    if (!g) return;
    memset(g,0,sizeof(*g));
    g->swipe_thresh = swipe_thresh>0?swipe_thresh:40; g->longpress_ms = longpress_ms>0?longpress_ms:500;
}
int touchgest_down(TouchGesture *g, int id, int x, int y, uint64_t now_ms) {
    if (!g || g->count>=TG_MAX_POINTS) return -1;
    tg_point *p=&g->pts[g->count++]; p->id=id; p->x=p->sx=x; p->y=p->sy=y; p->t0=now_ms; p->active=1; return 0;
}
int touchgest_move(TouchGesture *g, int id, int x, int y) {
    if (!g) return -1;
    for (int i=0;i<g->count;i++) if (g->pts[i].id==id && g->pts[i].active) { g->pts[i].x=x; g->pts[i].y=y; return 0; }
    return -1;
}
int touchgest_up(TouchGesture *g, int id, uint64_t now_ms) {
    if (!g) return TG_NONE;
    int idx=-1; for (int i=0;i<g->count;i++) if (g->pts[i].id==id) { idx=i; break; }
    if (idx<0) return TG_NONE;
    tg_point p=g->pts[idx];
    int dx=p.x-p.sx, dy=p.y-p.sy, adx=dx<0?-dx:dx, ady=dy<0?-dy:dy;
    int g_out=TG_NONE;
    if (adx<g->swipe_thresh && ady<g->swipe_thresh) {
        if (now_ms-p.t0>=(uint64_t)g->longpress_ms) g_out=TG_LONGPRESS;
        else if (p.t0-g->last_tap_ms < 300) g_out=TG_DOUBLETAP;
        else g_out=TG_TAP;
        g->last_tap_ms=now_ms;
    } else if (adx>ady) g_out = dx>0?TG_SWIPE_R:TG_SWIPE_L;
    else g_out = dy>0?TG_SWIPE_D:TG_SWIPE_U;
    /* remove point */
    memmove(&g->pts[idx],&g->pts[idx+1],sizeof(tg_point)*(g->count-idx-1)); g->count--;
    return g_out;
}
int touchgest_active_points(const TouchGesture *g) { return g?g->count:-1; }
