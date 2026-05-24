/*
 * puttyalt_aliases.c: Hostname alias resolution.
 */

#include "puttyalt_aliases.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void aliases_init(AliasStore *as)
{
    memset(as, 0, sizeof(*as));
}

static int find_alias(const AliasStore *as, const char *name)
{
    for (int i = 0; i < as->count; i++) {
        if (strcmp(as->entries[i].name, name) == 0)
            return i;
    }
    return -1;
}

const char *aliases_resolve(const AliasStore *as, const char *name)
{
    int idx = find_alias(as, name);
    if (idx < 0)
        return NULL;
    return as->entries[idx].value;
}

int aliases_set(AliasStore *as, const char *name, const char *value)
{
    if (!name || !value || !name[0])
        return -1;

    int idx = find_alias(as, name);
    if (idx >= 0) {
        snprintf(as->entries[idx].value, MAX_ALIAS_VALUE, "%s", value);
        return idx;
    }

    if (as->count >= MAX_ALIASES)
        return -1;

    HostAlias *e = &as->entries[as->count];
    snprintf(e->name, MAX_ALIAS_NAME, "%s", name);
    snprintf(e->value, MAX_ALIAS_VALUE, "%s", value);
    return as->count++;
}

int aliases_remove(AliasStore *as, const char *name)
{
    int idx = find_alias(as, name);
    if (idx < 0)
        return -1;

    for (int i = idx; i < as->count - 1; i++)
        as->entries[i] = as->entries[i + 1];
    as->count--;
    return 0;
}

int aliases_load(AliasStore *as, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];

    if (!f) return -1;

    aliases_init(as);

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\0')
            continue;

        /* Format: name=value */
        char *eq = strchr(line, '=');
        if (!eq)
            continue;

        *eq = '\0';
        const char *name = line;
        const char *value = eq + 1;

        /* Trim trailing spaces from name */
        char *end = eq - 1;
        while (end > line && isspace((unsigned char)*end))
            *end-- = '\0';

        /* Trim leading spaces from value */
        while (*value && isspace((unsigned char)*value))
            value++;

        if (*name && *value)
            aliases_set(as, name, value);
    }

    fclose(f);
    return 0;
}

int aliases_save(const AliasStore *as, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "# PuttyAlt host aliases\n");
    fprintf(f, "# Format: alias = user@host:port\n\n");

    for (int i = 0; i < as->count; i++) {
        fprintf(f, "%s = %s\n", as->entries[i].name, as->entries[i].value);
    }

    fclose(f);
    return 0;
}
