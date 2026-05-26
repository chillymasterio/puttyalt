#include <string.h>
#include <stdio.h>
#define MAX_MENU_ITEMS 64
typedef struct { char label[64]; char action[64]; int separator_before; int enabled; char shortcut[32]; } MenuItem;
static MenuItem g_items[MAX_MENU_ITEMS]; static int g_item_count = 0;
void contextmenu_clear(void) { g_item_count = 0; }
int contextmenu_add(const char *label, const char *action, const char *shortcut) {
    if (g_item_count >= MAX_MENU_ITEMS) return -1;
    MenuItem *m = &g_items[g_item_count]; memset(m, 0, sizeof(*m));
    snprintf(m->label, 64, "%s", label); snprintf(m->action, 64, "%s", action);
    if (shortcut) snprintf(m->shortcut, 32, "%s", shortcut); m->enabled = 1;
    return g_item_count++;
}
int contextmenu_add_separator(void) {
    if (g_item_count >= MAX_MENU_ITEMS) return -1;
    g_items[g_item_count].separator_before = 1; return 0;
}
void contextmenu_build_default(void) {
    contextmenu_clear();
    contextmenu_add("Copy", "copy", "Ctrl+Shift+C");
    contextmenu_add("Paste", "paste", "Ctrl+Shift+V");
    contextmenu_add_separator();
    contextmenu_add("Select All", "select_all", "Ctrl+Shift+A");
    contextmenu_add("Clear Terminal", "clear", NULL);
    contextmenu_add_separator();
    contextmenu_add("Find...", "find", "Ctrl+F");
    contextmenu_add("New Tab", "new_tab", "Ctrl+T");
    contextmenu_add_separator();
    contextmenu_add("Settings", "settings", "Ctrl+,");
}
int contextmenu_count(void) { return g_item_count; }
