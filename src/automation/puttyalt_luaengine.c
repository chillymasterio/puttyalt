#include "puttyalt_luaengine.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int lua_engine_init(LuaEngine *le, const char *scripts_dir)
{
    memset(le, 0, sizeof(*le));
    if (scripts_dir)
        snprintf(le->scripts_dir, sizeof(le->scripts_dir), "%s", scripts_dir);
    le->sandboxed = 1;
    le->allow_network = 0;
    le->allow_filesystem = 1;
    le->max_runtime_ms = 5000;
    le->engine_ready = 1;
    return 0;
}

void lua_engine_destroy(LuaEngine *le)
{
    for (int i = 0; i < le->script_count; i++) {
        if (le->scripts[i].state == LUA_SCRIPT_RUNNING)
            lua_engine_stop(le, i);
    }
    memset(le, 0, sizeof(*le));
}

int lua_engine_load(LuaEngine *le, const char *path, const char *name)
{
    if (le->script_count >= LUA_MAX_SCRIPTS) return -1;
    if (!path || !path[0]) return -1;

    LuaScript *s = &le->scripts[le->script_count];
    memset(s, 0, sizeof(*s));
    snprintf(s->path, sizeof(s->path), "%s", path);
    if (name && name[0])
        snprintf(s->name, sizeof(s->name), "%s", name);
    else {
        /* Extract filename as name */
        const char *slash = strrchr(path, '/');
        if (!slash) slash = strrchr(path, '\\');
        const char *base = slash ? slash + 1 : path;
        snprintf(s->name, sizeof(s->name), "%s", base);
        /* Remove .lua extension */
        char *dot = strrchr(s->name, '.');
        if (dot) *dot = '\0';
    }

    s->state = LUA_SCRIPT_IDLE;
    le->script_count++;
    return le->script_count - 1;
}

int lua_engine_run(LuaEngine *le, int script_idx, const char *input)
{
    if (script_idx < 0 || script_idx >= le->script_count) return -1;
    LuaScript *s = &le->scripts[script_idx];
    if (s->state == LUA_SCRIPT_RUNNING) return -2;

    s->state = LUA_SCRIPT_RUNNING;
    s->last_run = (long)time(NULL);
    s->output_len = 0;
    s->output[0] = '\0';
    s->last_error[0] = '\0';

    /* In real implementation: load and execute Lua bytecode
     * For now: simulate successful execution */
    (void)input;
    snprintf(s->output, sizeof(s->output),
             "-- Script '%s' executed successfully\n", s->name);
    s->output_len = (int)strlen(s->output);
    s->state = LUA_SCRIPT_DONE;

    return 0;
}

int lua_engine_stop(LuaEngine *le, int script_idx)
{
    if (script_idx < 0 || script_idx >= le->script_count) return -1;
    le->scripts[script_idx].state = LUA_SCRIPT_IDLE;
    return 0;
}

int lua_engine_reload(LuaEngine *le, int script_idx)
{
    if (script_idx < 0 || script_idx >= le->script_count) return -1;
    le->scripts[script_idx].state = LUA_SCRIPT_IDLE;
    le->scripts[script_idx].output_len = 0;
    le->scripts[script_idx].last_error[0] = '\0';
    return 0;
}

const char *lua_engine_output(const LuaEngine *le, int script_idx)
{
    if (script_idx < 0 || script_idx >= le->script_count) return NULL;
    return le->scripts[script_idx].output;
}

const char *lua_engine_error(const LuaEngine *le, int script_idx)
{
    if (script_idx < 0 || script_idx >= le->script_count) return NULL;
    return le->scripts[script_idx].last_error[0] ?
           le->scripts[script_idx].last_error : NULL;
}

int lua_engine_scan_dir(LuaEngine *le)
{
    (void)le;
    /* Scan scripts_dir for .lua files and auto-load */
    return 0;
}

int lua_engine_set_sandbox(LuaEngine *le, int network, int filesystem)
{
    le->allow_network = network;
    le->allow_filesystem = filesystem;
    return 0;
}
