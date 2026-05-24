#ifndef PUTTYALT_SNIPPETLIB_H
#define PUTTYALT_SNIPPETLIB_H

#define SNIPPET_MAX      256
#define SNIPPET_CAT_MAX  16

typedef struct {
    char name[64];
    char command[1024];
    char category[32];
    char description[128];
    char shortcut[16];
    int  usage_count;
    int  pinned;
} Snippet;

typedef struct {
    Snippet items[SNIPPET_MAX];
    int     count;
    char    categories[SNIPPET_CAT_MAX][32];
    int     cat_count;
    char    search_query[128];
} SnippetLibrary;

void snippetlib_init(SnippetLibrary *sl);
int  snippetlib_add(SnippetLibrary *sl, const char *name, const char *cmd, const char *cat);
int  snippetlib_remove(SnippetLibrary *sl, int index);
int  snippetlib_find(SnippetLibrary *sl, const char *name);
void snippetlib_sort_by_usage(SnippetLibrary *sl);
int  snippetlib_search(SnippetLibrary *sl, const char *query, int *results, int max_results);
int  snippetlib_save(SnippetLibrary *sl, const char *path);
int  snippetlib_load(SnippetLibrary *sl, const char *path);
void snippetlib_add_defaults(SnippetLibrary *sl);

#endif
