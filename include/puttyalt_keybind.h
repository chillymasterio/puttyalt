#ifndef PUTTYALT_KEYBIND_H
#define PUTTYALT_KEYBIND_H

#define KB_MAX_BINDINGS  256
#define KB_MAX_KEY       64
#define KB_MAX_ACTION    128

typedef enum {
    KB_MOD_NONE  = 0,
    KB_MOD_CTRL  = 1,
    KB_MOD_SHIFT = 2,
    KB_MOD_ALT   = 4,
    KB_MOD_META  = 8
} KBModifier;

typedef enum {
    KB_ACT_COPY = 0,
    KB_ACT_PASTE,
    KB_ACT_NEW_TAB,
    KB_ACT_CLOSE_TAB,
    KB_ACT_NEXT_TAB,
    KB_ACT_PREV_TAB,
    KB_ACT_SPLIT_H,
    KB_ACT_SPLIT_V,
    KB_ACT_ZOOM_IN,
    KB_ACT_ZOOM_OUT,
    KB_ACT_ZOOM_RESET,
    KB_ACT_FIND,
    KB_ACT_TOGGLE_SIDEBAR,
    KB_ACT_FULLSCREEN,
    KB_ACT_CLEAR_SCREEN,
    KB_ACT_SCROLL_UP,
    KB_ACT_SCROLL_DOWN,
    KB_ACT_SCROLL_TOP,
    KB_ACT_SCROLL_BOTTOM,
    KB_ACT_BROADCAST,
    KB_ACT_DISCONNECT,
    KB_ACT_RECONNECT,
    KB_ACT_SETTINGS,
    KB_ACT_CUSTOM,
    KB_ACT_COUNT
} KBAction;

typedef struct {
    int       keycode;
    int       modifiers;
    KBAction  action;
    char      custom_cmd[KB_MAX_ACTION];
    int       enabled;
} KBBinding;

typedef struct {
    KBBinding bindings[KB_MAX_BINDINGS];
    int       count;
    int       vi_mode;
} KeybindMgr;

void kb_init(KeybindMgr *km);
int  kb_add(KeybindMgr *km, int keycode, int modifiers, KBAction action);
int  kb_add_custom(KeybindMgr *km, int keycode, int modifiers, const char *cmd);
int  kb_remove(KeybindMgr *km, int index);
KBAction kb_lookup(const KeybindMgr *km, int keycode, int modifiers,
                   const char **custom_cmd);
int  kb_load(KeybindMgr *km, const char *path);
int  kb_save(const KeybindMgr *km, const char *path);
void kb_install_defaults(KeybindMgr *km);
const char *kb_action_name(KBAction action);

#endif
