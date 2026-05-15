#ifndef PUTTYALT_FAVORITES_H
#define PUTTYALT_FAVORITES_H

#define FAV_MAX 128
#define FAV_TAG_MAX 8

typedef struct {
    char name[128];
    char host[256];
    int port;
    char user[128];
    char keyfile[512];
    int protocol;
    char tags[FAV_TAG_MAX][32];
    int tag_count;
    int pinned;
    int color_index;
    unsigned long last_used;
    int use_count;
} Favorite;

typedef struct {
    Favorite items[FAV_MAX];
    int count;
    char filepath[512];
} FavoriteStore;

void favs_init(FavoriteStore *fs);
int  favs_load(FavoriteStore *fs, const char *path);
int  favs_save(const FavoriteStore *fs);
int  favs_add(FavoriteStore *fs, const Favorite *fav);
int  favs_remove(FavoriteStore *fs, int index);
Favorite *favs_get(FavoriteStore *fs, int index);
Favorite *favs_find_by_name(FavoriteStore *fs, const char *name);
int  favs_sort_by_usage(FavoriteStore *fs);
int  favs_sort_by_name(FavoriteStore *fs);
int  favs_count_with_tag(FavoriteStore *fs, const char *tag);

#endif
