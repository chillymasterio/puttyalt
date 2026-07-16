/* puttyalt_focusevent2.c - Focus in/out event tracking.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int focused; int reporting; long changes; } FocusEv;
void fcv_init(FocusEv *f){ if(f){f->focused=1;f->reporting=0;f->changes=0;} }
void fcv_set(FocusEv *f,int focused){ if(f&&f->focused!=focused){f->focused=focused;f->changes++;} }
const char *fcv_sequence(const FocusEv *f){ if(!f||!f->reporting)return ""; return f->focused?"\x1b[I":"\x1b[O"; }
