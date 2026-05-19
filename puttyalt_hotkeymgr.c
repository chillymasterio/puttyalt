#include "puttyalt_hotkeymgr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hkmgr_init(HotkeyMgr *hk)
{
    memset(hk, 0, sizeof(*hk));
    hk->enabled = 1;
}

int hkmgr_add(HotkeyMgr *hk, const char *name, int key, int mods,
              HKActionType action_type, const char *action)
{
    if (hk->count >= HK_MAX_BINDINGS) return -1;
    /* Check for conflict */
    int existing = hkmgr_find(hk, key, mods);
    if (existing >= 0) return -1;
    HKBinding *b = &hk->bindings[hk->count];
    memset(b, 0, sizeof(*b));
    snprintf(b->name, HK_MAX_NAME, "%s", name);
    b->key = key;
    b->modifiers = mods;
    b->action_type = action_type;
    snprintf(b->action, HK_MAX_CMD, "%s", action);
    b->enabled = 1;
    return hk->count++;
}

int hkmgr_remove(HotkeyMgr *hk, int index)
{
    if (index < 0 || index >= hk->count) return -1;
    for (int i = index; i < hk->count - 1; i++)
        hk->bindings[i] = hk->bindings[i + 1];
    hk->count--;
    return 0;
}

int hkmgr_find(const HotkeyMgr *hk, int key, int mods)
{
    for (int i = 0; i < hk->count; i++) {
        if (hk->bindings[i].key == key && hk->bindings[i].modifiers == mods
            && hk->bindings[i].enabled)
            return i;
    }
    return -1;
}

int hkmgr_trigger(HotkeyMgr *hk, int key, int mods)
{
    if (!hk->enabled) return -1;
    int idx = hkmgr_find(hk, key, mods);
    if (idx < 0) return -1;
    hk->bindings[idx].use_count++;
    return idx;
}

int hkmgr_load(HotkeyMgr *hk, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return -1;
    hkmgr_init(hk);
    HKBinding *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (strcmp(line, "[hotkey]") == 0) {
            if (hk->count >= HK_MAX_BINDINGS) break;
            cur = &hk->bindings[hk->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0) snprintf(cur->name, HK_MAX_NAME, "%s", line+5);
        else if (strncmp(line, "key=", 4) == 0) cur->key = atoi(line+4);
        else if (strncmp(line, "mods=", 5) == 0) cur->modifiers = atoi(line+5);
        else if (strncmp(line, "type=", 5) == 0) cur->action_type = atoi(line+5);
        else if (strncmp(line, "action=", 7) == 0) snprintf(cur->action, HK_MAX_CMD, "%s", line+7);
    }
    fclose(f);
    return 0;
}

int hkmgr_save(const HotkeyMgr *hk, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < hk->count; i++) {
        const HKBinding *b = &hk->bindings[i];
        fprintf(f, "[hotkey]\nname=%s\nkey=%d\nmods=%d\ntype=%d\naction=%s\n\n",
                b->name, b->key, b->modifiers, b->action_type, b->action);
    }
    fclose(f);
    return 0;
}

void hkmgr_load_defaults(HotkeyMgr *hk)
{
    hkmgr_add(hk, "Copy", 'C', HK_MOD_CTRL | HK_MOD_SHIFT, HK_ACT_INTERNAL, "clipboard.copy");
    hkmgr_add(hk, "Paste", 'V', HK_MOD_CTRL | HK_MOD_SHIFT, HK_ACT_INTERNAL, "clipboard.paste");
    hkmgr_add(hk, "New Tab", 'T', HK_MOD_CTRL | HK_MOD_SHIFT, HK_ACT_INTERNAL, "tab.new");
    hkmgr_add(hk, "Close Tab", 'W', HK_MOD_CTRL | HK_MOD_SHIFT, HK_ACT_INTERNAL, "tab.close");
    hkmgr_add(hk, "Find", 'F', HK_MOD_CTRL, HK_ACT_INTERNAL, "search.open");
    hkmgr_add(hk, "Zoom In", '+', HK_MOD_CTRL, HK_ACT_INTERNAL, "view.zoom_in");
    hkmgr_add(hk, "Zoom Out", '-', HK_MOD_CTRL, HK_ACT_INTERNAL, "view.zoom_out");
    hkmgr_add(hk, "Split Horizontal", 'H', HK_MOD_CTRL | HK_MOD_ALT, HK_ACT_INTERNAL, "split.horizontal");
    hkmgr_add(hk, "Split Vertical", 'V', HK_MOD_CTRL | HK_MOD_ALT, HK_ACT_INTERNAL, "split.vertical");
    hkmgr_add(hk, "Toggle Sidebar", 'B', HK_MOD_CTRL, HK_ACT_INTERNAL, "sidebar.toggle");
}

int hkmgr_export_cheatsheet(const HotkeyMgr *hk, char *buf, int bufsz)
{
    int pos = 0;
    pos += snprintf(buf + pos, bufsz - pos, "PuttyAlt Keyboard Shortcuts\n");
    pos += snprintf(buf + pos, bufsz - pos, "===========================\n\n");
    for (int i = 0; i < hk->count && pos < bufsz - 100; i++) {
        const HKBinding *b = &hk->bindings[i];
        char mods[32] = "";
        int mpos = 0;
        if (b->modifiers & HK_MOD_CTRL)
            mpos += snprintf(mods + mpos, sizeof(mods) - mpos, "Ctrl+");
        if (b->modifiers & HK_MOD_ALT)
            mpos += snprintf(mods + mpos, sizeof(mods) - mpos, "Alt+");
        if (b->modifiers & HK_MOD_SHIFT)
            mpos += snprintf(mods + mpos, sizeof(mods) - mpos, "Shift+");
        pos += snprintf(buf + pos, bufsz - pos, "  %-20s %s%c\n",
                       b->name, mods, b->key);
    }
    return pos;
}
