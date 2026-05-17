#include "puttyalt_envview.h"
#include <string.h>

void envview_init(EnvViewer *ev) { memset(ev, 0, sizeof(*ev)); }

int envview_add(EnvViewer *ev, const char *name, const char *value)
{
    if (ev->count >= ENV_MAX) return -1;
    EnvEntry *e = &ev->entries[ev->count++];
    snprintf(e->name, sizeof(e->name), "%s", name);
    snprintf(e->value, sizeof(e->value), "%s", value);
    return ev->count - 1;
}

int envview_set(EnvViewer *ev, const char *name, const char *value)
{
    for (int i = 0; i < ev->count; i++) {
        if (strcmp(ev->entries[i].name, name) == 0) {
            snprintf(ev->entries[i].value, sizeof(ev->entries[i].value), "%s", value);
            ev->entries[i].modified = 1;
            return i;
        }
    }
    return envview_add(ev, name, value);
}

const char *envview_get(EnvViewer *ev, const char *name)
{
    for (int i = 0; i < ev->count; i++)
        if (strcmp(ev->entries[i].name, name) == 0) return ev->entries[i].value;
    return NULL;
}

int envview_filter(EnvViewer *ev, const char *pattern, int *results, int max)
{
    int n = 0;
    for (int i = 0; i < ev->count && n < max; i++)
        if (strstr(ev->entries[i].name, pattern) || strstr(ev->entries[i].value, pattern))
            results[n++] = i;
    return n;
}

void envview_clear(EnvViewer *ev) { ev->count = 0; }
