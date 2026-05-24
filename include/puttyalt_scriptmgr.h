#ifndef PUTTYALT_SCRIPTMGR_H
#define PUTTYALT_SCRIPTMGR_H

#define SCRIPT_MAX_SCRIPTS  64
#define SCRIPT_NAME_LEN     128
#define SCRIPT_PATH_LEN     512
#define SCRIPT_MAX_ARGS     16

typedef enum {
    SCRIPT_SHELL,
    SCRIPT_PYTHON,
    SCRIPT_LUA,
    SCRIPT_EXPECT,
    SCRIPT_BATCH
} ScriptType;

typedef enum {
    SCRIPT_TRIGGER_MANUAL,
    SCRIPT_TRIGGER_ON_CONNECT,
    SCRIPT_TRIGGER_ON_DISCONNECT,
    SCRIPT_TRIGGER_ON_ERROR,
    SCRIPT_TRIGGER_SCHEDULED
} ScriptTrigger;

typedef struct {
    char name[SCRIPT_NAME_LEN];
    char path[SCRIPT_PATH_LEN];
    ScriptType type;
    ScriptTrigger trigger;
    int enabled;
    int timeout_sec;
    long last_run;
    int last_exit_code;
    char args[SCRIPT_MAX_ARGS][128];
    int num_args;
} ScriptEntry;

typedef struct {
    ScriptEntry scripts[SCRIPT_MAX_SCRIPTS];
    int count;
    char script_dir[SCRIPT_PATH_LEN];
    int auto_discover;
} ScriptManager;

void scriptmgr_init(ScriptManager *sm, const char *dir);
int  scriptmgr_add(ScriptManager *sm, const ScriptEntry *entry);
int  scriptmgr_remove(ScriptManager *sm, const char *name);
int  scriptmgr_run(ScriptManager *sm, const char *name);
int  scriptmgr_run_by_trigger(ScriptManager *sm, ScriptTrigger trigger);
const ScriptEntry *scriptmgr_find(const ScriptManager *sm, const char *name);
int  scriptmgr_discover(ScriptManager *sm);
int  scriptmgr_save(const ScriptManager *sm, const char *path);
int  scriptmgr_load(ScriptManager *sm, const char *path);

#endif
