/*
 * puttyalt_sniplib.c -- Shared snippet library implementation
 *
 * Manages collections of reusable command snippets that can be
 * shared across teams via git repositories or network paths.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "puttyalt_sniplib.h"

/*
 * Initialize a snippet library with a name and storage path.
 */
void sniplib_init(SnipLibrary *lib, const char *name, const char *path)
{
    if (!lib)
        return;

    memset(lib, 0, sizeof(SnipLibrary));

    if (name)
        snprintf(lib->name, SNIPLIB_MAX_NAME, "%s", name);
    if (path)
        snprintf(lib->path, SNIPLIB_MAX_PATH, "%s", path);

    lib->count = 0;
    lib->loaded = false;
    lib->git_url[0] = '\0';
}

/*
 * Load snippet library from a file on disk.
 * File format: line-based with name\tcontent per entry.
 */
bool sniplib_load(SnipLibrary *lib)
{
    FILE *fp;
    char line[SNIPLIB_MAX_NAME + SNIPLIB_MAX_CONTENT + 16];

    if (!lib || !lib->path[0])
        return false;

    fp = fopen(lib->path, "r");
    if (!fp)
        return false;

    lib->count = 0;

    while (fgets(line, sizeof(line), fp) && lib->count < SNIPLIB_MAX_SNIPPETS) {
        char *tab = strchr(line, '\t');
        if (!tab)
            continue;

        *tab = '\0';
        tab++;

        /* Strip trailing newline from content */
        size_t len = strlen(tab);
        if (len > 0 && tab[len - 1] == '\n')
            tab[len - 1] = '\0';

        snprintf(lib->entries[lib->count].name, SNIPLIB_MAX_NAME, "%s", line);
        snprintf(lib->entries[lib->count].content, SNIPLIB_MAX_CONTENT, "%s", tab);
        lib->entries[lib->count].active = true;
        lib->entries[lib->count].tag_count = 0;
        lib->count++;
    }

    fclose(fp);
    lib->loaded = true;
    return true;
}

/*
 * Save the snippet library to disk.
 */
bool sniplib_save(const SnipLibrary *lib)
{
    FILE *fp;
    int i;

    if (!lib || !lib->path[0])
        return false;

    fp = fopen(lib->path, "w");
    if (!fp)
        return false;

    for (i = 0; i < lib->count; i++) {
        if (!lib->entries[i].active)
            continue;
        fprintf(fp, "%s\t%s\n", lib->entries[i].name, lib->entries[i].content);
    }

    fclose(fp);
    return true;
}

/*
 * Add a new snippet to the library.
 * Returns the index of the new entry, or -1 if the library is full
 * or parameters are invalid.
 */
int sniplib_add(SnipLibrary *lib, const char *name, const char *content)
{
    int idx;

    if (!lib || !name || !content)
        return -1;

    if (lib->count >= SNIPLIB_MAX_SNIPPETS)
        return -1;

    /* Reject duplicates */
    if (sniplib_find(lib, name) >= 0)
        return -1;

    idx = lib->count;
    snprintf(lib->entries[idx].name, SNIPLIB_MAX_NAME, "%s", name);
    snprintf(lib->entries[idx].content, SNIPLIB_MAX_CONTENT, "%s", content);
    lib->entries[idx].active = true;
    lib->entries[idx].tag_count = 0;
    lib->count++;

    return idx;
}

/*
 * Remove a snippet by index.
 * Shifts remaining entries down to keep the array compact.
 */
bool sniplib_remove(SnipLibrary *lib, int index)
{
    if (!lib || index < 0 || index >= lib->count)
        return false;

    /* Shift entries down */
    for (int i = index; i < lib->count - 1; i++) {
        lib->entries[i] = lib->entries[i + 1];
    }

    lib->count--;
    memset(&lib->entries[lib->count], 0, sizeof(SnipEntry));

    return true;
}

/*
 * Find a snippet by name (case-sensitive).
 * Returns the index or -1 if not found.
 */
int sniplib_find(const SnipLibrary *lib, const char *name)
{
    if (!lib || !name)
        return -1;

    for (int i = 0; i < lib->count; i++) {
        if (lib->entries[i].active && strcmp(lib->entries[i].name, name) == 0)
            return i;
    }

    return -1;
}
