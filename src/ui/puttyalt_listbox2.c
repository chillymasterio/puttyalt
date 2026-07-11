/* puttyalt_listbox2.c - Listbox with multi-select.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define LB_MAX 64
typedef struct { unsigned char selected[LB_MAX]; int count; int cursor; } Listbox2;
void lbx_init(Listbox2 *l,int count){ if(!l)return; l->count=count>LB_MAX?LB_MAX:count; l->cursor=0; memset(l->selected,0,sizeof l->selected); }
void lbx_toggle(Listbox2 *l){ if(l&&l->cursor>=0&&l->cursor<l->count)l->selected[l->cursor]=!l->selected[l->cursor]; }
void lbx_move(Listbox2 *l,int delta){ if(!l)return; l->cursor+=delta; if(l->cursor<0)l->cursor=0; if(l->cursor>=l->count)l->cursor=l->count-1; }
int lbx_count_selected(const Listbox2 *l){ if(!l)return 0; int c=0; for(int i=0;i<l->count;i++)if(l->selected[i])c++; return c; }
