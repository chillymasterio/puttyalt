#ifndef PUTTYALT_LUAENGINE_H
#define PUTTYALT_LUAENGINE_H

#define LUA_MAX_SCRIPTS     64
#define LUA_MAX_PATH        512
#define LUA_MAX_OUTPUT      4096

typedef enum {
    LUA_SCRIPT_IDLE,
    LUA_SCRIPT_RUNNING,
    LUA_SCRIPT_ERROR,
    LUA_SCRIPT_DONE
} LuaScriptState;

typedef struct {
    char name[64];
    char path[LUA_MAX_PATH];
    char description[128];
    LuaScriptState state;
    char last_error[256];
    char output[LUA_MAX_OUTPUT];
    int output_len;
    long last_run;
    int auto_run;
    int hotkey_mod;
    int hotkey_key;
} LuaScript;

typedef struct {
    LuaScript scripts[LUA_MAX_SCRIPTS];
    int script_count;
    int engine_ready;
    int sandboxed;
    char scripts_dir[LUA_MAX_PATH];
    int allow_network;
    int allow_filesystem;
    int max_runtime_ms;
} LuaEngine;

int  lua_engine_init(LuaEngine *le, const char *scripts_dir);
void lua_engine_destroy(LuaEngine *le);
int  lua_engine_load(LuaEngine *le, const char *path, const char *name);
int  lua_engine_run(LuaEngine *le, int script_idx, const char *input);
int  lua_engine_stop(LuaEngine *le, int script_idx);
int  lua_engine_reload(LuaEngine *le, int script_idx);
const char *lua_engine_output(const LuaEngine *le, int script_idx);
const char *lua_engine_error(const LuaEngine *le, int script_idx);
int  lua_engine_scan_dir(LuaEngine *le);
int  lua_engine_set_sandbox(LuaEngine *le, int network, int filesystem);

#endif
