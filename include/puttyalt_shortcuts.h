/*
 * puttyalt_shortcuts.h: Customisable keyboard shortcuts.
 *
 * Maps key combinations to actions.  Allows user customisation
 * via the config file.
 */

#ifndef PUTTYALT_SHORTCUTS_H
#define PUTTYALT_SHORTCUTS_H

#define MAX_SHORTCUTS 128

typedef enum {
    ACTION_NONE = 0,
    ACTION_NEW_TAB,
    ACTION_CLOSE_TAB,
    ACTION_NEXT_TAB,
    ACTION_PREV_TAB,
    ACTION_TAB_1, ACTION_TAB_2, ACTION_TAB_3, ACTION_TAB_4,
    ACTION_TAB_5, ACTION_TAB_6, ACTION_TAB_7, ACTION_TAB_8,
    ACTION_TAB_9,
    ACTION_QUICK_CONNECT,
    ACTION_TOGGLE_SFTP,
    ACTION_SEARCH,
    ACTION_PASTE_DIALOG,
    ACTION_BROADCAST_TOGGLE,
    ACTION_FULLSCREEN,
    ACTION_ZOOM_IN,
    ACTION_ZOOM_OUT,
    ACTION_ZOOM_RESET,
    ACTION_COPY,
    ACTION_PASTE,
    ACTION_DUPLICATE_TAB,
    ACTION_SETTINGS,
    ACTION_THEME_CYCLE,
    ACTION_COUNT
} ShortcutAction;

typedef struct KeyBinding {
    int modifier;       /* bitmask: 1=Ctrl, 2=Shift, 4=Alt */
    int keycode;        /* virtual key code */
    ShortcutAction action;
} KeyBinding;

typedef struct ShortcutManager {
    KeyBinding bindings[MAX_SHORTCUTS];
    int count;
} ShortcutManager;

void shortcuts_init(ShortcutManager *sm);
void shortcuts_load_defaults(ShortcutManager *sm);
int  shortcuts_add(ShortcutManager *sm, int modifier, int keycode, ShortcutAction action);
ShortcutAction shortcuts_lookup(const ShortcutManager *sm, int modifier, int keycode);
int  shortcuts_load(ShortcutManager *sm, const char *path);
int  shortcuts_save(const ShortcutManager *sm, const char *path);
const char *shortcuts_action_name(ShortcutAction action);

#endif /* PUTTYALT_SHORTCUTS_H */
