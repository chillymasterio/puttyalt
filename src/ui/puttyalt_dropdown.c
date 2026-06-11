/* puttyalt_dropdown.c - Dropdown/combobox state with keyboard nav. */
#include <string.h>
#include <stdio.h>
#define DD_MAX 64
#define DD_TEXT 64
typedef struct { char items[DD_MAX][DD_TEXT]; int n; int selected; int open; int highlighted; } Dropdown;
void dropdown_init(Dropdown *d) { if(d){ memset(d,0,sizeof(*d)); d->selected=-1; d->highlighted=-1; } }
int dropdown_add(Dropdown *d, const char *item) {
    if(!d||d->n>=DD_MAX||!item) return -1;
    snprintf(d->items[d->n],DD_TEXT,"%s",item); if(d->selected<0)d->selected=0; return d->n++;
}
void dropdown_toggle(Dropdown *d) { if(d){ d->open=!d->open; if(d->open) d->highlighted=d->selected; } }
int dropdown_move(Dropdown *d, int delta) {
    if(!d||!d->open||d->n==0) return -1;
    d->highlighted+=delta; if(d->highlighted<0)d->highlighted=0; if(d->highlighted>=d->n)d->highlighted=d->n-1;
    return d->highlighted;
}
int dropdown_commit(Dropdown *d) {
    if(!d||!d->open) return -1; d->selected=d->highlighted; d->open=0; return d->selected;
}
const char *dropdown_selected_text(const Dropdown *d) {
    return (d&&d->selected>=0&&d->selected<d->n)?d->items[d->selected]:"";
}
int dropdown_count(const Dropdown *d) { return d?d->n:-1; }
