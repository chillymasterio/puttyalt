/* puttyalt_contextbar.c - Contextual action bar (changes by terminal state). */
#include <string.h>
#include <stdio.h>
#define CB_MAX 16
#define CB_LABEL 24
typedef struct { char label[CB_LABEL]; int action_id; int state_mask; } cb_button;
typedef struct { cb_button buttons[CB_MAX]; int n; int current_state; } ContextBar;
void contextbar_init(ContextBar *c) { if(c) memset(c,0,sizeof(*c)); }
int contextbar_add(ContextBar *c, const char *label, int action_id, int state_mask) {
    if(!c||c->n>=CB_MAX||!label) return -1;
    cb_button *b=&c->buttons[c->n++]; snprintf(b->label,CB_LABEL,"%s",label); b->action_id=action_id; b->state_mask=state_mask;
    return 0;
}
void contextbar_set_state(ContextBar *c, int state) { if(c) c->current_state=state; }
int contextbar_visible_buttons(const ContextBar *c, int *out_idx, int cap) {
    if(!c) return -1;
    int n=0;
    for (int i=0;i<c->n && n<cap;i++) if (c->buttons[i].state_mask & c->current_state) out_idx[n++]=i;
    return n;
}
const char *contextbar_label(const ContextBar *c, int idx) {
    return (c&&idx>=0&&idx<c->n)?c->buttons[idx].label:0;
}
int contextbar_count(const ContextBar *c) { return c?c->n:-1; }
