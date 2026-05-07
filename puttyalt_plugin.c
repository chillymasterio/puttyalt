#include <string.h>
#include "puttyalt_plugin.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

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

#ifndef _WIN32
    p->handle = dlopen(path, RTLD_LAZY);
    if (!p->handle) return -1;

    /* Look up required symbols */
    *(void **)(&p->init) = dlsym(p->handle, "puttyalt_plugin_init");
    *(void **)(&p->cleanup) = dlsym(p->handle, "puttyalt_plugin_cleanup");

    /* Optional hooks */
    *(void **)(&p->on_connect) = dlsym(p->handle, "puttyalt_on_connect");
    *(void **)(&p->on_disconnect) = dlsym(p->handle, "puttyalt_on_disconnect");
    *(void **)(&p->on_data) = dlsym(p->handle, "puttyalt_on_data");

    /* Get plugin info */
    PluginInfo *(*get_info)(void) = NULL;
    *(void **)(&get_info) = dlsym(p->handle, "puttyalt_plugin_info");
    if (get_info) {
        PluginInfo *info = get_info();
        if (info) p->info = *info;
    }
#else
    p->handle = LoadLibraryA(path);
    if (!p->handle) return -1;
    p->init = (int(*)(void))GetProcAddress(p->handle, "puttyalt_plugin_init");
    p->cleanup = (void(*)(void))GetProcAddress(p->handle, "puttyalt_plugin_cleanup");
#endif

    if (p->init && p->init() != 0) {
        plugin_unload(pm, pm->count);
        return -1;
    }

    p->state = PLUGIN_LOADED;
    return pm->count++;
}

int plugin_unload(PluginManager *pm, int index)
{
    if (index < 0 || index >= pm->count) return -1;
    Plugin *p = &pm->plugins[index];

    if (p->cleanup) p->cleanup();

#ifndef _WIN32
    if (p->handle) dlclose(p->handle);
#else
    if (p->handle) FreeLibrary(p->handle);
#endif

    p->state = PLUGIN_UNLOADED;
    p->handle = NULL;
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
