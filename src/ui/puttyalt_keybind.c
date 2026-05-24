#include "puttyalt_keybind.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *action_names[] = {
    "Copy", "Paste", "New Tab", "Close Tab", "Next Tab", "Prev Tab",
    "Split Horizontal", "Split Vertical", "Zoom In", "Zoom Out", "Zoom Reset",
    "Find", "Toggle Sidebar", "Fullscreen", "Clear Screen",
    "Scroll Up", "Scroll Down", "Scroll Top", "Scroll Bottom",
    "Broadcast", "Disconnect", "Reconnect", "Settings", "Custom"
};

void kb_init(KeybindMgr *km)
{
    memset(km, 0, sizeof(*km));
    kb_install_defaults(km);
}

int kb_add(KeybindMgr *km, int keycode, int modifiers, KBAction action)
{
    if (km->count >= KB_MAX_BINDINGS) return -1;
    KBBinding *b = &km->bindings[km->count];
    b->keycode = keycode;
    b->modifiers = modifiers;
    b->action = action;
    b->enabled = 1;
    b->custom_cmd[0] = '\0';
    return km->count++;
}

int kb_add_custom(KeybindMgr *km, int keycode, int modifiers, const char *cmd)
{
    if (km->count >= KB_MAX_BINDINGS) return -1;
    KBBinding *b = &km->bindings[km->count];
    b->keycode = keycode;
    b->modifiers = modifiers;
    b->action = KB_ACT_CUSTOM;
    b->enabled = 1;
    snprintf(b->custom_cmd, KB_MAX_ACTION, "%s", cmd);
    return km->count++;
}

int kb_remove(KeybindMgr *km, int index)
{
    if (index < 0 || index >= km->count) return -1;
    for (int i = index; i < km->count - 1; i++)
        km->bindings[i] = km->bindings[i + 1];
    km->count--;
    return 0;
}

KBAction kb_lookup(const KeybindMgr *km, int keycode, int modifiers,
                   const char **custom_cmd)
{
    for (int i = 0; i < km->count; i++) {
        const KBBinding *b = &km->bindings[i];
        if (b->enabled && b->keycode == keycode && b->modifiers == modifiers) {
            if (custom_cmd && b->action == KB_ACT_CUSTOM)
                *custom_cmd = b->custom_cmd;
            return b->action;
        }
    }
    return KB_ACT_COUNT; /* not found */
}

int kb_load(KeybindMgr *km, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    KBBinding *cur = NULL;
    if (!f) return -1;
    km->count = 0;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[bind]") == 0) {
            if (km->count >= KB_MAX_BINDINGS) break;
            cur = &km->bindings[km->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "key=", 4) == 0)
            cur->keycode = (int)strtol(line + 4, NULL, 0);
        else if (strncmp(line, "mod=", 4) == 0)
            cur->modifiers = atoi(line + 4);
        else if (strncmp(line, "action=", 7) == 0)
            cur->action = atoi(line + 7);
        else if (strncmp(line, "cmd=", 4) == 0)
            snprintf(cur->custom_cmd, KB_MAX_ACTION, "%s", line + 4);
    }
    fclose(f);
    return 0;
}

int kb_save(const KeybindMgr *km, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < km->count; i++) {
        const KBBinding *b = &km->bindings[i];
        fprintf(f, "[bind]\nkey=0x%X\nmod=%d\naction=%d\n",
                b->keycode, b->modifiers, b->action);
        if (b->action == KB_ACT_CUSTOM)
            fprintf(f, "cmd=%s\n", b->custom_cmd);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

void kb_install_defaults(KeybindMgr *km)
{
    /* Ctrl+C = Copy, Ctrl+V = Paste, Ctrl+T = New Tab, etc. */
    kb_add(km, 'C', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_COPY);
    kb_add(km, 'V', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_PASTE);
    kb_add(km, 'T', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_NEW_TAB);
    kb_add(km, 'W', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_CLOSE_TAB);
    kb_add(km, 0x09, KB_MOD_CTRL, KB_ACT_NEXT_TAB);  /* Ctrl+Tab */
    kb_add(km, 0x09, KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_PREV_TAB);
    kb_add(km, 'D', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_SPLIT_H);
    kb_add(km, 'E', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_SPLIT_V);
    kb_add(km, '=', KB_MOD_CTRL, KB_ACT_ZOOM_IN);
    kb_add(km, '-', KB_MOD_CTRL, KB_ACT_ZOOM_OUT);
    kb_add(km, '0', KB_MOD_CTRL, KB_ACT_ZOOM_RESET);
    kb_add(km, 'F', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_FIND);
    kb_add(km, 'B', KB_MOD_CTRL | KB_MOD_SHIFT, KB_ACT_TOGGLE_SIDEBAR);
    kb_add(km, 0x7A, KB_MOD_NONE, KB_ACT_FULLSCREEN); /* F11 */
    kb_add(km, 'L', KB_MOD_CTRL, KB_ACT_CLEAR_SCREEN);
}

const char *kb_action_name(KBAction action)
{
    if (action >= 0 && action < KB_ACT_COUNT)
        return action_names[action];
    return "Unknown";
}
