#include <string.h>
#include <stdio.h>

#define MAX_BINDS 128
#define KEY_NAME_LEN 32
#define ACTION_LEN 64

typedef struct { char key[KEY_NAME_LEN]; char action[ACTION_LEN]; int enabled; } KeyBind;
static KeyBind g_binds[MAX_BINDS];
static int g_bind_count = 0;

static void add_default(const char *key, const char *action)
{
    if (g_bind_count >= MAX_BINDS) return;
    KeyBind *b = &g_binds[g_bind_count++];
    snprintf(b->key, KEY_NAME_LEN, "%s", key);
    snprintf(b->action, ACTION_LEN, "%s", action);
    b->enabled = 1;
}

void keybinds_init(void)
{
    g_bind_count = 0;
    add_default("Ctrl+P",       "command_palette");
    add_default("Ctrl+T",       "new_tab");
    add_default("Ctrl+W",       "close_tab");
    add_default("Ctrl+Tab",     "next_tab");
    add_default("Ctrl+Shift+T", "reopen_tab");
    add_default("Ctrl+N",       "new_session");
    add_default("Ctrl+D",       "duplicate_session");
    add_default("Ctrl+F",       "find");
    add_default("Ctrl+S",       "save_session");
    add_default("Ctrl+Shift+S", "save_all");
    add_default("Ctrl+,",       "settings");
    add_default("F11",          "fullscreen");
    add_default("Ctrl+Plus",    "zoom_in");
    add_default("Ctrl+Minus",   "zoom_out");
    add_default("Ctrl+0",       "zoom_reset");
    add_default("Ctrl+Shift+C", "copy");
    add_default("Ctrl+Shift+V", "paste");
    add_default("Ctrl+Shift+N", "new_window");
}

int keybinds_remap(const char *key, const char *action)
{
    for (int i = 0; i < g_bind_count; i++)
        if (strcmp(g_binds[i].key, key) == 0) {
            snprintf(g_binds[i].action, ACTION_LEN, "%s", action);
            return 0;
        }
    if (g_bind_count >= MAX_BINDS) return -1;
    add_default(key, action);
    return 0;
}

const char *keybinds_get_action(const char *key)
{
    for (int i = 0; i < g_bind_count; i++)
        if (g_binds[i].enabled && strcmp(g_binds[i].key, key) == 0)
            return g_binds[i].action;
    return NULL;
}

int keybinds_toggle(const char *key, int enabled)
{
    for (int i = 0; i < g_bind_count; i++)
        if (strcmp(g_binds[i].key, key) == 0) { g_binds[i].enabled = enabled; return 0; }
    return -1;
}
