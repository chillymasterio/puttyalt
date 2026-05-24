#include "puttyalt_shortcutoverlay.h"
#include <string.h>
#include <stdio.h>

int shortcut_overlay_init(ShortcutOverlay *so)
{
    memset(so, 0, sizeof(*so));
    so->columns = 3;
    so->opacity = 0.95f;

    /* Pre-populate with default shortcuts */
    shortcut_overlay_add(so, "Ctrl+N", "New session", "File");
    shortcut_overlay_add(so, "Ctrl+O", "Open session", "File");
    shortcut_overlay_add(so, "Ctrl+S", "Save session", "File");
    shortcut_overlay_add(so, "Ctrl+D", "Duplicate tab", "File");
    shortcut_overlay_add(so, "Ctrl+Shift+C", "Copy", "Edit");
    shortcut_overlay_add(so, "Ctrl+Shift+V", "Paste", "Edit");
    shortcut_overlay_add(so, "Ctrl+F", "Find", "Edit");
    shortcut_overlay_add(so, "Ctrl+,", "Settings", "Edit");
    shortcut_overlay_add(so, "Ctrl+R", "Reconnect", "Session");
    shortcut_overlay_add(so, "Ctrl+B", "Broadcast", "Session");
    shortcut_overlay_add(so, "F11", "Fullscreen", "View");
    shortcut_overlay_add(so, "Ctrl++/-/0", "Zoom", "View");
    shortcut_overlay_add(so, "Ctrl+\\", "Toggle sidebar", "View");
    shortcut_overlay_add(so, "Ctrl+Shift+P", "Command palette", "Tools");
    shortcut_overlay_add(so, "Ctrl+Tab", "Next tab", "Nav");
    shortcut_overlay_add(so, "F1", "Help", "Help");
    return 0;
}

void shortcut_overlay_add(ShortcutOverlay *so, const char *key,
                          const char *desc, const char *category)
{
    if (so->count >= SC_MAX_ENTRIES) return;
    ShortcutEntry *e = &so->entries[so->count];
    snprintf(e->key, sizeof(e->key), "%s", key);
    snprintf(e->description, sizeof(e->description), "%s", desc);
    if (category) snprintf(e->category, sizeof(e->category), "%s", category);
    so->count++;
}

void shortcut_overlay_toggle(ShortcutOverlay *so)
{
    so->visible = !so->visible;
}

void shortcut_overlay_show(ShortcutOverlay *so) { so->visible = 1; }
void shortcut_overlay_hide(ShortcutOverlay *so) { so->visible = 0; }
int shortcut_overlay_visible(const ShortcutOverlay *so) { return so->visible; }
