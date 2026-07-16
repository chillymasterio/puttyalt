/* puttyalt_mousemode2.c - Mouse tracking mode state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { MOUSE_OFF, MOUSE_X10, MOUSE_NORMAL, MOUSE_BUTTON, MOUSE_ANY };
typedef struct { int mode; int sgr; int utf8; } MouseMode;
void mmd_init(MouseMode *m){ if(m){m->mode=MOUSE_OFF;m->sgr=0;m->utf8=0;} }
void mmd_set(MouseMode *m,int mode){ if(m)m->mode=mode; }
int mmd_reports_motion(const MouseMode *m){ return m&&(m->mode==MOUSE_BUTTON||m->mode==MOUSE_ANY); }
int mmd_enabled(const MouseMode *m){ return m&&m->mode!=MOUSE_OFF; }
