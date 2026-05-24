/*
 * puttyalt_snippets.h: Saved command snippets for quick execution.
 *
 * Sysadmins frequently type the same commands: restart services,
 * check logs, deploy code. Snippets let you save and recall these
 * commands with keyboard shortcuts or from a menu.
 */

#ifndef PUTTYALT_SNIPPETS_H
#define PUTTYALT_SNIPPETS_H

#define MAX_SNIPPET_NAME   64
#define MAX_SNIPPET_CMD    4096
#define MAX_SNIPPETS       256

typedef struct Snippet {
    char name[MAX_SNIPPET_NAME];
    char command[MAX_SNIPPET_CMD];
    char category[MAX_SNIPPET_NAME];
    int send_enter;     /* automatically press Enter after pasting */
} Snippet;

typedef struct SnippetStore {
    Snippet snippets[MAX_SNIPPETS];
    int count;
    char config_path[4096];
} SnippetStore;

/* Load snippets from file */
int snippets_load(SnippetStore *store, const char *path);

/* Save snippets to file */
int snippets_save(const SnippetStore *store);

/* Add a new snippet */
int snippets_add(SnippetStore *store, const Snippet *snip);

/* Remove a snippet by name */
int snippets_remove(SnippetStore *store, const char *name);

/* Find snippet by name (returns NULL if not found) */
const Snippet *snippets_find(const SnippetStore *store, const char *name);

/* List snippets in a category */
int snippets_list_category(const SnippetStore *store, const char *category,
                           const Snippet **results, int max_results);

#endif /* PUTTYALT_SNIPPETS_H */
