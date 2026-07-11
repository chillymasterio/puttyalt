/* puttyalt_modal3.c - Modal dialog focus/result state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
enum { MOD_NONE, MOD_OK, MOD_CANCEL, MOD_YES, MOD_NO };
typedef struct { int open; int result; int focus; int nbuttons; } Modal3;
void mdl_open(Modal3 *m,int nbuttons){ if(m){m->open=1;m->result=MOD_NONE;m->focus=0;m->nbuttons=nbuttons;} }
void mdl_focus_next(Modal3 *m){ if(m&&m->nbuttons)m->focus=(m->focus+1)%m->nbuttons; }
void mdl_close(Modal3 *m,int result){ if(m){m->open=0;m->result=result;} }
