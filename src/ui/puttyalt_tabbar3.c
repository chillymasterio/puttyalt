/* puttyalt_tabbar3.c - Tab bar with active/scroll state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int count; int active; int scroll; int visible; } TabBar3;
void tbr_init(TabBar3 *t,int visible){ if(t){t->count=0;t->active=0;t->scroll=0;t->visible=visible;} }
void tbr_activate(TabBar3 *t,int idx){ if(!t||idx<0||idx>=t->count)return; t->active=idx; if(idx<t->scroll)t->scroll=idx; if(idx>=t->scroll+t->visible)t->scroll=idx-t->visible+1; }
int tbr_is_visible(const TabBar3 *t,int idx){ return t&&idx>=t->scroll&&idx<t->scroll+t->visible; }
