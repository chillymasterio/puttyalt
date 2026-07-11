/* puttyalt_radiogroup.c - Radio button group selection.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define RG_MAX 16
typedef struct { int count; int selected; } RadioGroup;
void rdg_init(RadioGroup *r,int count){ if(r){r->count=count>RG_MAX?RG_MAX:count;r->selected=0;} }
void rdg_select(RadioGroup *r,int idx){ if(r&&idx>=0&&idx<r->count)r->selected=idx; }
void rdg_next(RadioGroup *r){ if(r&&r->count)r->selected=(r->selected+1)%r->count; }
int rdg_is_selected(const RadioGroup *r,int idx){ return r&&r->selected==idx; }
