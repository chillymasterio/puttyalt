#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "puttyalt_bookmarks.h"

void bm_init(BookmarkStore *store)
{
    memset(store, 0, sizeof(*store));
}

int bm_add(BookmarkStore *store, const char *name, const char *host,
           int port, const char *user)
{
    if (store->count >= BM_MAX) return -1;
    if (bm_find(store, name) >= 0) return -1;
    Bookmark *b = &store->items[store->count];
    memset(b, 0, sizeof(*b));
    strncpy(b->name, name, BM_NAME_LEN - 1);
    strncpy(b->host, host, sizeof(b->host) - 1);
    b->port = port;
    if (user) strncpy(b->username, user, sizeof(b->username) - 1);
    b->sort_order = store->count;
    return store->count++;
}

int bm_remove(BookmarkStore *store, int index)
{
    if (index < 0 || index >= store->count) return -1;
    for (int i = index; i < store->count - 1; i++)
        store->items[i] = store->items[i + 1];
    store->count--;
    return 0;
}

int bm_find(const BookmarkStore *store, const char *name)
{
    for (int i = 0; i < store->count; i++)
        if (strcmp(store->items[i].name, name) == 0) return i;
    return -1;
}

int bm_set_folder(BookmarkStore *store, int index, const char *folder)
{
    if (index < 0 || index >= store->count) return -1;
    strncpy(store->items[index].folder, folder, sizeof(store->items[0].folder) - 1);
    return 0;
}

static int cmp_name(const void *a, const void *b)
{
    return strcmp(((const Bookmark *)a)->name, ((const Bookmark *)b)->name);
}

static int cmp_recent(const void *a, const void *b)
{
    const Bookmark *ba = a, *bb = b;
    if (bb->last_used > ba->last_used) return 1;
    if (bb->last_used < ba->last_used) return -1;
    return 0;
}

int bm_sort_by_name(BookmarkStore *store)
{
    if (store->count <= 1) return 0;
    qsort(store->items, store->count, sizeof(Bookmark), cmp_name);
    return 0;
}

int bm_sort_by_recent(BookmarkStore *store)
{
    if (store->count <= 1) return 0;
    qsort(store->items, store->count, sizeof(Bookmark), cmp_recent);
    return 0;
}

int bm_save(const BookmarkStore *store, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return -1;
    for (int i = 0; i < store->count; i++) {
        const Bookmark *b = &store->items[i];
        fprintf(fp, "[bookmark]\nname=%s\nhost=%s\nport=%d\nuser=%s\nfolder=%s\n\n",
                b->name, b->host, b->port, b->username, b->folder);
    }
    fclose(fp);
    return 0;
}

int bm_load(BookmarkStore *store, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    char line[512];
    Bookmark *cur = NULL;
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[bookmark]") == 0) {
            if (store->count >= BM_MAX) break;
            cur = &store->items[store->count++];
            memset(cur, 0, sizeof(*cur));
        } else if (cur) {
            if (strncmp(line, "name=", 5) == 0)
                strncpy(cur->name, line+5, BM_NAME_LEN - 1);
            else if (strncmp(line, "host=", 5) == 0)
                strncpy(cur->host, line+5, sizeof(cur->host) - 1);
            else if (strncmp(line, "port=", 5) == 0)
                cur->port = atoi(line+5);
            else if (strncmp(line, "user=", 5) == 0)
                strncpy(cur->username, line+5, sizeof(cur->username) - 1);
            else if (strncmp(line, "folder=", 7) == 0)
                strncpy(cur->folder, line+7, sizeof(cur->folder) - 1);
        }
    }
    fclose(fp);
    return store->count;
}
