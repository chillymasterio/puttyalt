/*
 * puttyalt_aliases.h: Hostname aliases.
 *
 * Maps short names to full connection strings so users can type
 * "prod1" in the quick-connect bar instead of "admin@10.0.5.42:2222".
 * Aliases are stored in the portable config directory.
 */

#ifndef PUTTYALT_ALIASES_H
#define PUTTYALT_ALIASES_H

#define MAX_ALIASES     256
#define MAX_ALIAS_NAME   64
#define MAX_ALIAS_VALUE 512

typedef struct HostAlias {
    char name[MAX_ALIAS_NAME];     /* short name, e.g. "prod1" */
    char value[MAX_ALIAS_VALUE];   /* full target, e.g. "admin@10.0.5.42:2222" */
} HostAlias;

typedef struct AliasStore {
    HostAlias entries[MAX_ALIASES];
    int count;
} AliasStore;

void aliases_init(AliasStore *as);

/* Resolve an alias.  Returns the expanded value, or NULL if not found. */
const char *aliases_resolve(const AliasStore *as, const char *name);

/* Add or update an alias.  Returns index or -1 on error. */
int aliases_set(AliasStore *as, const char *name, const char *value);

/* Remove an alias by name.  Returns 0 on success. */
int aliases_remove(AliasStore *as, const char *name);

/* Load / save from config file */
int aliases_load(AliasStore *as, const char *path);
int aliases_save(const AliasStore *as, const char *path);

#endif /* PUTTYALT_ALIASES_H */
