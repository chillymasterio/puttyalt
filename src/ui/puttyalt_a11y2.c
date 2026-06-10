/* puttyalt_a11y2.c - Accessibility v2: focus order + ARIA-like roles. */
#include <string.h>
#include <stdio.h>
#define AY_MAX 64
#define AY_LABEL 64
enum ay_role { AY_NONE=0, AY_BUTTON, AY_TEXTBOX, AY_LIST, AY_TAB, AY_MENU, AY_DIALOG, AY_TERMINAL };
typedef struct { int id; int role; char label[AY_LABEL]; int focusable; int focus_order; int disabled; } ay_element;
typedef struct { ay_element elements[AY_MAX]; int n; int focused; } A11y2;
void a11y2_init(A11y2 *a) { if(a){ memset(a,0,sizeof(*a)); a->focused=-1; } }
int a11y2_register(A11y2 *a, int id, int role, const char *label, int focusable) {
    if(!a||a->n>=AY_MAX) return -1;
    ay_element *e=&a->elements[a->n]; e->id=id; e->role=role; snprintf(e->label,AY_LABEL,"%s",label?label:"");
    e->focusable=focusable?1:0; e->focus_order=a->n; return a->n++;
}
int a11y2_focus_next(A11y2 *a) {
    if(!a||a->n==0) return -1;
    int start=a->focused;
    for (int k=1;k<=a->n;k++) {
        int i=(start+k)%a->n;
        if (a->elements[i].focusable && !a->elements[i].disabled) { a->focused=i; return a->elements[i].id; }
    }
    return -1;
}
const char *a11y2_describe(const A11y2 *a, int idx) {
    if(!a||idx<0||idx>=a->n) return "";
    return a->elements[idx].label;
}
int a11y2_role_at(const A11y2 *a, int idx) { return (a&&idx>=0&&idx<a->n)?a->elements[idx].role:-1; }
int a11y2_focused_id(const A11y2 *a) { return (a&&a->focused>=0)?a->elements[a->focused].id:-1; }
int a11y2_count(const A11y2 *a) { return a?a->n:-1; }
