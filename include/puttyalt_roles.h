#ifndef PUTTYALT_ROLES_H
#define PUTTYALT_ROLES_H

#define ROLE_MAX_PROFILES   16
#define ROLE_MAX_RULES      64
#define ROLE_MAX_NAME       48
#define ROLE_MAX_PATTERN    128

typedef enum {
    ROLE_ALLOW,
    ROLE_DENY,
    ROLE_WARN
} RoleAction;

typedef enum {
    RULE_HOST_PATTERN,
    RULE_PORT_RANGE,
    RULE_COMMAND_FILTER,
    RULE_TIME_WINDOW,
    RULE_MAX_SESSIONS
} RuleType;

typedef struct {
    RuleType type;
    RoleAction action;
    char pattern[ROLE_MAX_PATTERN];
    int value_min;
    int value_max;
} RoleRule;

typedef struct {
    char name[ROLE_MAX_NAME];
    char description[128];
    RoleRule rules[ROLE_MAX_RULES];
    int rule_count;
    int active;
    int locked;  /* can't be modified by user */
} RoleProfile;

typedef struct {
    RoleProfile profiles[ROLE_MAX_PROFILES];
    int profile_count;
    int active_profile;
    int enforcement;  /* 0=off, 1=warn, 2=enforce */
} RoleManager;

int  role_init(RoleManager *rm);
void role_destroy(RoleManager *rm);
int  role_add_profile(RoleManager *rm, const char *name, const char *desc);
int  role_add_rule(RoleManager *rm, int profile_idx, RuleType type,
                   RoleAction action, const char *pattern, int min, int max);
RoleAction role_check_host(const RoleManager *rm, const char *host, int port);
RoleAction role_check_command(const RoleManager *rm, const char *cmd);
int  role_activate(RoleManager *rm, int profile_idx);
int  role_load(RoleManager *rm, const char *path);
int  role_save(const RoleManager *rm, const char *path);

#endif
