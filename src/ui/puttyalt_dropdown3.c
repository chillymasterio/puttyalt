/* puttyalt_dropdown3.c - Dropdown selection with open state.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int count; int selected; int open; int highlight; } Dropdown3;
void dd3_init(Dropdown3 *d,int count){ if(d){d->count=count;d->selected=0;d->open=0;d->highlight=0;} }
void dd3_toggle(Dropdown3 *d){ if(d){d->open=!d->open; if(d->open)d->highlight=d->selected;} }
void dd3_move(Dropdown3 *d,int delta){ if(!d||!d->open)return; d->highlight+=delta; if(d->highlight<0)d->highlight=0; if(d->highlight>=d->count)d->highlight=d->count-1; }
void dd3_commit(Dropdown3 *d){ if(d&&d->open){d->selected=d->highlight;d->open=0;} }
