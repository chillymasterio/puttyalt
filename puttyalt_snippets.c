/*
 * puttyalt_snippets.c: Saved command snippet store.
 */

#include "puttyalt_snippets.h"
#include <stdio.h>
#include <string.h>

int snippets_load(SnippetStore *store, const char *path)
{
    FILE *f;
    char line[8192];

    memset(store, 0, sizeof(*store));
    snprintf(store->config_path, sizeof(store->config_path), "%s", path);

    f = fopen(path, "r");
    if (!f)
        return -1;

    Snippet *cur = NULL;

    while (fgets(line, sizeof(line), f)) {
        /* Strip newline */
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (strcmp(line, "[snippet]") == 0) {
            if (store->count >= MAX_SNIPPETS) break;
            cur = &store->snippets[store->count++];
            memset(cur, 0, sizeof(*cur));
            cur->send_enter = 1;
            continue;
        }

        if (!cur) continue;

        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, sizeof(cur->name), "%s", line + 5);
        else if (strncmp(line, "command=", 8) == 0)
            snprintf(cur->command, sizeof(cur->command), "%s", line + 8);
        else if (strncmp(line, "category=", 9) == 0)
            snprintf(cur->category, sizeof(cur->category), "%s", line + 9);
        else if (strncmp(line, "send_enter=", 11) == 0)
            cur->send_enter = atoi(line + 11);
    }

    fclose(f);
    return 0;
}

int snippets_save(const SnippetStore *store)
{
    FILE *f = fopen(store->config_path, "w");
    if (!f) return -1;

    for (int i = 0; i < store->count; i++) {
        const Snippet *s = &store->snippets[i];
        fprintf(f, "[snippet]\n");
        fprintf(f, "name=%s\n", s->name);
        fprintf(f, "command=%s\n", s->command);
        fprintf(f, "category=%s\n", s->category);
        fprintf(f, "send_enter=%d\n", s->send_enter);
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}

int snippets_add(SnippetStore *store, const Snippet *snip)
{
    if (store->count >= MAX_SNIPPETS)
        return -1;
    memcpy(&store->snippets[store->count++], snip, sizeof(Snippet));
    return 0;
}

int snippets_remove(SnippetStore *store, const char *name)
{
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->snippets[i].name, name) == 0) {
            /* Shift remaining snippets down */
            for (int j = i; j < store->count - 1; j++)
                store->snippets[j] = store->snippets[j + 1];
            store->count--;
            return 0;
        }
    }
    return -1;
}

const Snippet *snippets_find(const SnippetStore *store, const char *name)
{
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->snippets[i].name, name) == 0)
            return &store->snippets[i];
    }
    return NULL;
}

int snippets_list_category(const SnippetStore *store, const char *category,
                           const Snippet **results, int max_results)
{
    int n = 0;
    for (int i = 0; i < store->count && n < max_results; i++) {
        if (strcmp(store->snippets[i].category, category) == 0)
            results[n++] = &store->snippets[i];
    }
    return n;
}
