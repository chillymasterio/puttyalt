#ifndef PUTTYALT_PLUGIN_H
#define PUTTYALT_PLUGIN_H

#define PLUGIN_MAX        16
#define PLUGIN_NAME_LEN   64
#define PLUGIN_VER_LEN    16
#define PLUGIN_API_VER    1

typedef enum {
    PLUGIN_UNLOADED = 0,
    PLUGIN_LOADED,
    PLUGIN_ACTIVE,
    PLUGIN_ERROR
} PluginState;

typedef struct PluginInfo {
    char name[PLUGIN_NAME_LEN];
    char version[PLUGIN_VER_LEN];
    char author[128];
    char description[256];
    int api_version;
} PluginInfo;

typedef struct Plugin {
    PluginInfo info;
    PluginState state;
    void *handle;          /* dlopen/LoadLibrary handle */
    int (*init)(void);
    void (*cleanup)(void);
    int (*on_connect)(const char *host, int port);
    int (*on_disconnect)(void);
    int (*on_data)(const unsigned char *data, int len);
} Plugin;

typedef struct PluginManager {
    Plugin plugins[PLUGIN_MAX];
    int count;
    int enabled;
} PluginManager;

void plugin_mgr_init(PluginManager *pm);
int  plugin_load(PluginManager *pm, const char *path);
int  plugin_unload(PluginManager *pm, int index);
int  plugin_enable(PluginManager *pm, int index);
int  plugin_disable(PluginManager *pm, int index);
void plugin_notify_connect(PluginManager *pm, const char *host, int port);
void plugin_notify_disconnect(PluginManager *pm);
void plugin_notify_data(PluginManager *pm, const unsigned char *data, int len);

#endif
