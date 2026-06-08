/* puttyalt_mouse2.c - Mouse protocol v2 (SGR pixels, all modes). */
#include <string.h>
#include <stdio.h>
enum m2_mode { M2_OFF=0, M2_X10=9, M2_NORMAL=1000, M2_BUTTON=1002, M2_ANY=1003, M2_SGR=1006, M2_PIXEL=1016 };
typedef struct { int mode; int sgr; int pixel; } Mouse2;
void mouse2_init(Mouse2 *m) { if(m) memset(m,0,sizeof(*m)); }
void mouse2_set_mode(Mouse2 *m, int mode, int enable) {
    if(!m) return;
    if (mode==M2_SGR) { m->sgr=enable; return; }
    if (mode==M2_PIXEL) { m->pixel=enable; if(enable) m->sgr=1; return; }
    m->mode = enable ? mode : M2_OFF;
}
int mouse2_encode(const Mouse2 *m, int button, int x, int y, int pressed, char *out, int outlen) {
    if(!m||!out||m->mode==M2_OFF) return -1;
    if (m->sgr) {
        return snprintf(out,outlen,"\x1b[<%d;%d;%d%c",button,x,y,pressed?'M':'m');
    }
    /* legacy X10 encoding */
    int cb=button+32, cx=x+32, cy=y+32;
    return snprintf(out,outlen,"\x1b[M%c%c%c",cb,cx>255?255:cx,cy>255?255:cy);
}
int mouse2_wants_motion(const Mouse2 *m) {
    return (m && (m->mode==M2_BUTTON||m->mode==M2_ANY))?1:0;
}
int mouse2_active(const Mouse2 *m) { return (m&&m->mode!=M2_OFF)?1:0; }
