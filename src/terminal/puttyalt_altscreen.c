/* puttyalt_altscreen.c - Alternate screen buffer state + cursor save/restore. */
#include <string.h>
typedef struct { int alt_active; int saved_cx, saved_cy; int main_cx, main_cy; int saved_attr; int switches; } AltScreen;
void altscreen_init(AltScreen *a) { if(a) memset(a,0,sizeof(*a)); }
int altscreen_enter(AltScreen *a, int cx, int cy, int attr) {
    if(!a||a->alt_active) return -1;
    a->main_cx=cx; a->main_cy=cy; a->saved_attr=attr; a->alt_active=1; a->switches++; return 0;
}
int altscreen_leave(AltScreen *a, int *out_cx, int *out_cy) {
    if(!a||!a->alt_active) return -1;
    a->alt_active=0; if(out_cx)*out_cx=a->main_cx; if(out_cy)*out_cy=a->main_cy; return 0;
}
void altscreen_save_cursor(AltScreen *a, int cx, int cy) { if(a){ a->saved_cx=cx; a->saved_cy=cy; } }
int altscreen_restore_cursor(const AltScreen *a, int *cx, int *cy) {
    if(!a) return -1;
    if(cx) *cx=a->saved_cx;
    if(cy)*cy=a->saved_cy; return 0;
}
int altscreen_is_active(const AltScreen *a) { return a?a->alt_active:-1; }
