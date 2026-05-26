#include <string.h>
#include <stdio.h>
#define MAX_CATEGORIES 32
#define CAT_NAME_LEN 64
#define MAX_SNIPPETS_PER_CAT 64
typedef struct { char name[128]; char content[1024]; } CatSnippet;
typedef struct { char name[CAT_NAME_LEN]; CatSnippet snippets[MAX_SNIPPETS_PER_CAT]; int count; } SnipCategory;
static SnipCategory g_cats[MAX_CATEGORIES]; static int g_cat_count = 0;
int snipcategory_create(const char *name) {
    if (g_cat_count >= MAX_CATEGORIES) return -1;
    memset(&g_cats[g_cat_count], 0, sizeof(SnipCategory));
    snprintf(g_cats[g_cat_count].name, CAT_NAME_LEN, "%s", name);
    return g_cat_count++;
}
int snipcategory_add_snippet(int cat_id, const char *name, const char *content) {
    if (cat_id < 0 || cat_id >= g_cat_count) return -1;
    SnipCategory *c = &g_cats[cat_id]; if (c->count >= MAX_SNIPPETS_PER_CAT) return -1;
    snprintf(c->snippets[c->count].name, 128, "%s", name);
    snprintf(c->snippets[c->count].content, 1024, "%s", content);
    c->count++; return 0;
}
int snipcategory_search(const char *query, char results[][128], int max) {
    int n = 0;
    for (int i = 0; i < g_cat_count && n < max; i++)
        for (int j = 0; j < g_cats[i].count && n < max; j++)
            if (strstr(g_cats[i].snippets[j].name, query) || strstr(g_cats[i].snippets[j].content, query))
                snprintf(results[n++], 128, "%s/%s", g_cats[i].name, g_cats[i].snippets[j].name);
    return n;
}
