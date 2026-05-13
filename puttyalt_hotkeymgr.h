#ifndef PUTTYALT_HOTKEYMGR_H
#define PUTTYALT_HOTKEYMGR_H

#define HK_MAX_BINDINGS 128
#define HK_MAX_NAME     64
#define HK_MAX_CMD      256

typedef enum {
    HK_MOD_NONE  = 0,
    HK_MOD_CTRL  = 1,
    HK_MOD_ALT   = 2,
    HK_MOD_SHIFT = 4,
    HK_MOD_WIN   = 8
} HKModifier;

typedef enum {
    HK_ACT_COMMAND = 0,
    HK_ACT_MENU,
    HK_ACT_MACRO,
    HK_ACT_SCRIPT,
    HK_ACT_INTERNAL
} HKActionType;

typedef struct {
    char        name[HK_MAX_NAME];
    int         key;
    int         modifiers;
    HKActionType action_type;
    char        action[HK_MAX_CMD];
    int         enabled;
    int         global;
    long        use_count;
} HKBinding;

typedef struct {
    HKBinding bindings[HK_MAX_BINDINGS];
    int       count;
    int       enabled;
} HotkeyMgr;

void hkmgr_init(HotkeyMgr *hk);
int  hkmgr_add(HotkeyMgr *hk, const char *name, int key, int mods,
               HKActionType action_type, const char *action);
int  hkmgr_remove(HotkeyMgr *hk, int index);
int  hkmgr_find(const HotkeyMgr *hk, int key, int mods);
int  hkmgr_trigger(HotkeyMgr *hk, int key, int mods);
int  hkmgr_load(HotkeyMgr *hk, const char *path);
int  hkmgr_save(const HotkeyMgr *hk, const char *path);
int  hkmgr_export_cheatsheet(const HotkeyMgr *hk, char *buf, int bufsz);
void hkmgr_load_defaults(HotkeyMgr *hk);

#endif
