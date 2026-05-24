#ifndef PUTTYALT_BOOKMARKS_H
#define PUTTYALT_BOOKMARKS_H

#define BM_MAX        64
#define BM_NAME_LEN   128

typedef struct Bookmark {
    char name[BM_NAME_LEN];
    char host[256];
    int port;
    char username[128];
    char folder[64];
    int sort_order;
    unsigned long last_used;
} Bookmark;

typedef struct BookmarkStore {
    Bookmark items[BM_MAX];
    int count;
    char file_path[512];
} BookmarkStore;

void bm_init(BookmarkStore *store);
int  bm_add(BookmarkStore *store, const char *name, const char *host,
            int port, const char *user);
int  bm_remove(BookmarkStore *store, int index);
int  bm_find(const BookmarkStore *store, const char *name);
int  bm_set_folder(BookmarkStore *store, int index, const char *folder);
int  bm_save(const BookmarkStore *store, const char *path);
int  bm_load(BookmarkStore *store, const char *path);
int  bm_sort_by_name(BookmarkStore *store);
int  bm_sort_by_recent(BookmarkStore *store);

#endif
