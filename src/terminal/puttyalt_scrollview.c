/* puttyalt_scrollview.c - Scroll viewport over content.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int top; int height; int total; } ScrollView;
void svw_init(ScrollView *v,int height){ if(v){v->top=0;v->height=height;v->total=0;} }
void svw_scroll(ScrollView *v,int delta){ if(!v)return; v->top+=delta; int max=v->total-v->height; if(max<0)max=0; if(v->top>max)v->top=max; if(v->top<0)v->top=0; }
void svw_to_bottom(ScrollView *v){ if(!v)return; v->top=v->total-v->height; if(v->top<0)v->top=0; }
int svw_at_bottom(const ScrollView *v){ return v && v->top>=v->total-v->height; }
