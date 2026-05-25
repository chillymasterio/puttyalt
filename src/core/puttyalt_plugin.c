#include <string.h>
#include <stdio.h>
#include "puttyalt_plugin.h"

/*
 * Plugin manager: manages registered plugin metadata.
 * Plugin manager: manages registered plugin metadata.
 * Currently plugins are compiled-in.
 */

void plugin_mgr_init(PluginManager *pm)
{
    memset(pm, 0, sizeof(*pm));
    pm->enabled = 1;
}

int plugin_load(PluginManager *pm, const char *path)
{
    if (pm->count >= PLUGIN_MAX) return -1;

    Plugin *p = &pm->plugins[pm->count];
    memset(p, 0, sizeof(*p));

    /* Store path for future dynamic loading support */
    snprintf(p->info.name, sizeof(p->info.name), "%s", path);
    p->state = PLUGIN_LOADED;

    return pm->count++;
}

int plugin_unload(PluginManager *pm, int index)
{
    if (index < 0 || index >= pm->count) return -1;
    Plugin *p = &pm->plugins[index];

    if (p->cleanup) p->cleanup();

    p->state = PLUGIN_UNLOADED;
    p->handle_id = 0;
    return 0;
}

int plugin_enable(PluginManager *pm, int index)
{
    if (index < 0 || index >= pm->count) return -1;
    pm->plugins[index].state = PLUGIN_ACTIVE;
    return 0;
}

int plugin_disable(PluginManager *pm, int index)
{
    if (index < 0 || index >= pm->count) return -1;
    pm->plugins[index].state = PLUGIN_LOADED;
    return 0;
}

void plugin_notify_connect(PluginManager *pm, const char *host, int port)
{
    if (!pm->enabled) return;
    for (int i = 0; i < pm->count; i++) {
        Plugin *p = &pm->plugins[i];
        if (p->state == PLUGIN_ACTIVE && p->on_connect)
            p->on_connect(host, port);
    }
}

void plugin_notify_disconnect(PluginManager *pm)
{
    if (!pm->enabled) return;
    for (int i = 0; i < pm->count; i++) {
        Plugin *p = &pm->plugins[i];
        if (p->state == PLUGIN_ACTIVE && p->on_disconnect)
            p->on_disconnect();
    }
}

void plugin_notify_data(PluginManager *pm, const unsigned char *data, int len)
{
    if (!pm->enabled) return;
    for (int i = 0; i < pm->count; i++) {
        Plugin *p = &pm->plugins[i];
        if (p->state == PLUGIN_ACTIVE && p->on_data)
            p->on_data(data, len);
    }
}
