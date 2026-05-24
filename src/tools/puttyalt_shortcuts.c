/*
 * puttyalt_shortcuts.c: Keyboard shortcut implementation.
 */

#include "puttyalt_shortcuts.h"
#include <stdio.h>
#include <string.h>

void shortcuts_init(ShortcutManager *sm)
{
    memset(sm, 0, sizeof(*sm));
}

int shortcuts_add(ShortcutManager *sm, int modifier, int keycode, ShortcutAction action)
{
    if (sm->count >= MAX_SHORTCUTS)
        return -1;
    KeyBinding *kb = &sm->bindings[sm->count];
    kb->modifier = modifier;
    kb->keycode = keycode;
    kb->action = action;
    return sm->count++;
}

void shortcuts_load_defaults(ShortcutManager *sm)
{
    shortcuts_init(sm);
    /* Ctrl+T = new tab */
    shortcuts_add(sm, 1, 'T', ACTION_NEW_TAB);
    /* Ctrl+W = close tab */
    shortcuts_add(sm, 1, 'W', ACTION_CLOSE_TAB);
    /* Ctrl+Tab = next tab */
    shortcuts_add(sm, 1, 0x09, ACTION_NEXT_TAB);
    /* Ctrl+Shift+Tab = prev tab */
    shortcuts_add(sm, 3, 0x09, ACTION_PREV_TAB);
    /* Ctrl+K = quick connect */
    shortcuts_add(sm, 1, 'K', ACTION_QUICK_CONNECT);
    /* Ctrl+E = toggle SFTP panel */
    shortcuts_add(sm, 1, 'E', ACTION_TOGGLE_SFTP);
    /* Ctrl+Shift+F = search */
    shortcuts_add(sm, 3, 'F', ACTION_SEARCH);
    /* Ctrl+Shift+B = broadcast toggle */
    shortcuts_add(sm, 3, 'B', ACTION_BROADCAST_TOGGLE);
    /* F11 = fullscreen */
    shortcuts_add(sm, 0, 0x7A, ACTION_FULLSCREEN);
    /* Ctrl+Plus = zoom in */
    shortcuts_add(sm, 1, 0xBB, ACTION_ZOOM_IN);
    /* Ctrl+Minus = zoom out */
    shortcuts_add(sm, 1, 0xBD, ACTION_ZOOM_OUT);
    /* Ctrl+0 = zoom reset */
    shortcuts_add(sm, 1, '0', ACTION_ZOOM_RESET);
    /* Ctrl+1..9 = switch to tab N */
    for (int i = 1; i <= 9; i++)
        shortcuts_add(sm, 1, '0' + i, ACTION_TAB_1 + (i - 1));
}

ShortcutAction shortcuts_lookup(const ShortcutManager *sm, int modifier, int keycode)
{
    for (int i = 0; i < sm->count; i++) {
        if (sm->bindings[i].modifier == modifier &&
            sm->bindings[i].keycode == keycode)
            return sm->bindings[i].action;
    }
    return ACTION_NONE;
}

const char *shortcuts_action_name(ShortcutAction action)
{
    switch (action) {
    case ACTION_NEW_TAB: return "New Tab";
    case ACTION_CLOSE_TAB: return "Close Tab";
    case ACTION_NEXT_TAB: return "Next Tab";
    case ACTION_PREV_TAB: return "Previous Tab";
    case ACTION_QUICK_CONNECT: return "Quick Connect";
    case ACTION_TOGGLE_SFTP: return "Toggle SFTP Panel";
    case ACTION_SEARCH: return "Search";
    case ACTION_BROADCAST_TOGGLE: return "Toggle Broadcast";
    case ACTION_FULLSCREEN: return "Fullscreen";
    case ACTION_ZOOM_IN: return "Zoom In";
    case ACTION_ZOOM_OUT: return "Zoom Out";
    case ACTION_ZOOM_RESET: return "Zoom Reset";
    case ACTION_COPY: return "Copy";
    case ACTION_PASTE: return "Paste";
    case ACTION_DUPLICATE_TAB: return "Duplicate Tab";
    case ACTION_SETTINGS: return "Settings";
    case ACTION_THEME_CYCLE: return "Cycle Theme";
    default: return "Unknown";
    }
}

int shortcuts_load(ShortcutManager *sm, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    if (!f) return -1;

    shortcuts_init(sm);

    while (fgets(line, sizeof(line), f)) {
        int mod, key, act;
        if (sscanf(line, "%d %d %d", &mod, &key, &act) == 3)
            shortcuts_add(sm, mod, key, (ShortcutAction)act);
    }
    fclose(f);
    return 0;
}

int shortcuts_save(const ShortcutManager *sm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < sm->count; i++)
        fprintf(f, "%d %d %d\n", sm->bindings[i].modifier,
                sm->bindings[i].keycode, sm->bindings[i].action);
    fclose(f);
    return 0;
}
