/* puttyalt_menubar.c - Menu bar model with submenus + accelerators. */
#include <string.h>
#include <stdio.h>
#define MB_MAX 64
#define MB_LABEL 40
typedef struct { char label[MB_LABEL]; int parent; int action_id; char accel[16]; int separator; int enabled; } mb_item;
typedef struct { mb_item items[MB_MAX]; int n; } MenuBar;
void menubar_init(MenuBar *m) { if(m) memset(m,0,sizeof(*m)); }
int menubar_add(MenuBar *m, const char *label, int parent, int action_id, const char *accel) {
    if(!m||m->n>=MB_MAX) return -1;
    mb_item *it=&m->items[m->n]; snprintf(it->label,MB_LABEL,"%s",label?label:""); it->parent=parent;
    it->action_id=action_id; snprintf(it->accel,16,"%s",accel?accel:""); it->enabled=1; return m->n++;
}
int menubar_add_separator(MenuBar *m, int parent) {
    if(!m||m->n>=MB_MAX) return -1;
    m->items[m->n].parent=parent; m->items[m->n].separator=1; return m->n++;
}
int menubar_children(const MenuBar *m, int parent, int *out_idx, int cap) {
    if(!m) return -1;
    int n=0;
    for (int i=0;i<m->n && n<cap;i++) if (m->items[i].parent==parent) out_idx[n++]=i;
    return n;
}
int menubar_find_accel(const MenuBar *m, const char *accel) {
    if(!m||!accel) return -1;
    for (int i=0;i<m->n;i++) if (m->items[i].enabled && strcmp(m->items[i].accel,accel)==0) return m->items[i].action_id;
    return -1;
}
void menubar_set_enabled(MenuBar *m, int idx, int on) { if(m&&idx>=0&&idx<m->n) m->items[idx].enabled=on?1:0; }
int menubar_count(const MenuBar *m) { return m?m->n:-1; }
