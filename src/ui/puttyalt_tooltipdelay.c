/* puttyalt_tooltipdelay.c - Tooltip delay and placement state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { long hover_start; long delay; int visible; } Tooltip3;
void tt5_init(Tooltip3 *t,long delay){ if(t){t->hover_start=0;t->delay=delay;t->visible=0;} }
void tt5_hover(Tooltip3 *t,long now){ if(t&&t->hover_start==0)t->hover_start=now; }
void tt5_leave(Tooltip3 *t){ if(t){t->hover_start=0;t->visible=0;} }
int tt5_update(Tooltip3 *t,long now){ if(!t||t->hover_start==0)return 0; if(now-t->hover_start>=t->delay)t->visible=1; return t->visible; }
