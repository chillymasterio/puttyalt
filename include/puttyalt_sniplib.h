/*
 * puttyalt_sniplib.h -- Shared snippet library management
 *
 * Provides team-shared snippet collections that can be synchronized
 * via git repositories or network shares.
 */

#ifndef PUTTYALT_SNIPLIB_H
#define PUTTYALT_SNIPLIB_H

#include <stdbool.h>

#define SNIPLIB_MAX_SNIPPETS   256
#define SNIPLIB_MAX_NAME       128
#define SNIPLIB_MAX_PATH       512
#define SNIPLIB_MAX_CONTENT    4096
#define SNIPLIB_MAX_TAG        64
#define SNIPLIB_MAX_TAGS       8

/* A single snippet entry */
typedef struct SnipEntry {
    char name[SNIPLIB_MAX_NAME];
    char content[SNIPLIB_MAX_CONTENT];
    char tags[SNIPLIB_MAX_TAGS][SNIPLIB_MAX_TAG];
    int tag_count;
    bool active;
} SnipEntry;

/* A snippet library (collection of entries) */
typedef struct SnipLibrary {
    char name[SNIPLIB_MAX_NAME];
    char path[SNIPLIB_MAX_PATH];
    char git_url[SNIPLIB_MAX_PATH];
    SnipEntry entries[SNIPLIB_MAX_SNIPPETS];
    int count;
    bool loaded;
} SnipLibrary;

/* Initialize a snippet library structure */
void sniplib_init(SnipLibrary *lib, const char *name, const char *path);

/* Load library entries from disk */
bool sniplib_load(SnipLibrary *lib);

/* Save library entries to disk */
bool sniplib_save(const SnipLibrary *lib);

/* Add a snippet to the library; returns index or -1 on failure */
int sniplib_add(SnipLibrary *lib, const char *name, const char *content);

/* Remove a snippet by index; returns true on success */
bool sniplib_remove(SnipLibrary *lib, int index);

/* Find a snippet by name; returns index or -1 if not found */
int sniplib_find(const SnipLibrary *lib, const char *name);

#endif /* PUTTYALT_SNIPLIB_H */
