#ifndef PUTTYALT_ENVMGR_H
#define PUTTYALT_ENVMGR_H

#define ENV_MAX_VARS    128
#define ENV_MAX_NAME    64
#define ENV_MAX_VALUE   512
#define ENV_MAX_PROFILES 16
#define ENV_MAX_PNAME   64

typedef struct {
    char name[ENV_MAX_NAME];
    char value[ENV_MAX_VALUE];
    int  exported;
    int  sensitive;     /* mask in UI */
} EnvVar;

typedef struct {
    char    name[ENV_MAX_PNAME];
    EnvVar  vars[ENV_MAX_VARS];
    int     count;
} EnvProfile;

typedef struct {
    EnvProfile profiles[ENV_MAX_PROFILES];
    int        profile_count;
    int        active;
} EnvMgr;

void envmgr_init(EnvMgr *em);
int  envmgr_add_profile(EnvMgr *em, const char *name);
int  envmgr_remove_profile(EnvMgr *em, int index);
int  envmgr_set_active(EnvMgr *em, int index);
int  envmgr_set_var(EnvMgr *em, int profile, const char *name, const char *value, int sensitive);
int  envmgr_unset_var(EnvMgr *em, int profile, const char *name);
const char *envmgr_get_var(const EnvMgr *em, int profile, const char *name);
int  envmgr_export(const EnvMgr *em, int profile, char *buf, int bufsz);
int  envmgr_load(EnvMgr *em, const char *path);
int  envmgr_save(const EnvMgr *em, const char *path);

#endif
