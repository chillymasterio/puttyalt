#include <string.h>
#include <stdio.h>
typedef enum { MOUSE_NONE, MOUSE_X10, MOUSE_VT200, MOUSE_BTN_EVENT, MOUSE_ANY_EVENT, MOUSE_SGR } MouseMode;
static MouseMode g_mode = MOUSE_NONE;
static int g_focus_tracking = 0;
void mousetrack_set_mode(MouseMode mode) { g_mode = mode; }
MouseMode mousetrack_get_mode(void) { return g_mode; }
int mousetrack_encode_x10(int btn, int x, int y, char *buf, int buflen) {
    return snprintf(buf, buflen, "\x1b[M%c%c%c", (char)(32+btn), (char)(32+x+1), (char)(32+y+1));
}
int mousetrack_encode_sgr(int btn, int x, int y, int release, char *buf, int buflen) {
    return snprintf(buf, buflen, "\x1b[<%d;%d;%d%c", btn, x+1, y+1, release?'m':'M');
}
void mousetrack_set_focus_tracking(int on) { g_focus_tracking = on; }
int mousetrack_focus_in(char *buf, int buflen) { return g_focus_tracking ? snprintf(buf, buflen, "\x1b[I") : 0; }
int mousetrack_focus_out(char *buf, int buflen) { return g_focus_tracking ? snprintf(buf, buflen, "\x1b[O") : 0; }
