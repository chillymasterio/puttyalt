#ifndef PUTTYALT_SHORTCUTOVERLAY_H
#define PUTTYALT_SHORTCUTOVERLAY_H

#define SC_MAX_ENTRIES 48

typedef struct {
    char key[24];
    char description[64];
    char category[32];
} ShortcutEntry;

typedef struct {
    ShortcutEntry entries[SC_MAX_ENTRIES];
    int count;
    int visible;
    float opacity;
    int columns;
} ShortcutOverlay;

int  shortcut_overlay_init(ShortcutOverlay *so);
void shortcut_overlay_add(ShortcutOverlay *so, const char *key,
                          const char *desc, const char *category);
void shortcut_overlay_toggle(ShortcutOverlay *so);
void shortcut_overlay_show(ShortcutOverlay *so);
void shortcut_overlay_hide(ShortcutOverlay *so);
int  shortcut_overlay_visible(const ShortcutOverlay *so);

#endif
