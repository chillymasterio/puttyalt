/* puttyalt_tabcontext.c - Tab context menu builder (per-tab actions). */
#include <string.h>
#include <stdio.h>
#define TC_MAX 16
#define TC_LABEL 32
typedef struct {
    char label[TC_LABEL];
    int action_id;
    int enabled;
    int separator;
} tc_item;
typedef struct {
    tc_item items[TC_MAX];
    int n;
    int target_tab;
} TabContext;
void tabcontext_init(TabContext *t, int target_tab) {
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->target_tab = target_tab;
}
int tabcontext_add(TabContext *t, const char *label, int action_id, int enabled) {
    if (!t || t->n >= TC_MAX) return -1;
    tc_item *it = &t->items[t->n];
    snprintf(it->label, TC_LABEL, "%s", label ? label : "");
    it->action_id = action_id;
    it->enabled = enabled ? 1 : 0;
    return t->n++;
}
int tabcontext_add_separator(TabContext *t) {
    if (!t || t->n >= TC_MAX) return -1;
    t->items[t->n].separator = 1;
    return t->n++;
}
int tabcontext_action_at(const TabContext *t, int idx) {
    if (!t || idx < 0 || idx >= t->n) return -1;
    if (t->items[idx].separator || !t->items[idx].enabled) return -1;
    return t->items[idx].action_id;
}
int tabcontext_count(const TabContext *t) {
    return t ? t->n : -1;
}
