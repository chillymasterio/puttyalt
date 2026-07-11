/* puttyalt_checkbox.c - Checkbox widget state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int checked; int enabled; } Checkbox;
void chk_init(Checkbox *c,int checked){ if(c){c->checked=checked;c->enabled=1;} }
void chk_toggle(Checkbox *c){ if(c&&c->enabled)c->checked=!c->checked; }
const char *chk_glyph(const Checkbox *c){ if(!c)return "[ ]"; return c->checked?"[x]":"[ ]"; }
