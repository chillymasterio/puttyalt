#include "puttyalt_favorites.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void favs_init(FavoriteStore *fs) { memset(fs, 0, sizeof(*fs)); }

int favs_load(FavoriteStore *fs, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    snprintf(fs->filepath, sizeof(fs->filepath), "%s", path);
    char line[1024]; fs->count = 0;
    Favorite *cur = NULL;
    while (fgets(line, sizeof(line), f) && fs->count < FAV_MAX) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '[') {
            cur = &fs->items[fs->count++];
            memset(cur, 0, sizeof(*cur));
            sscanf(line, "[%127[^]]", cur->name);
        } else if (cur) {
            char *eq = strchr(line, '=');
            if (!eq) continue;
            *eq = '\0';
            char *val = eq + 1;
            if (strcmp(line, "host") == 0) snprintf(cur->host, sizeof(cur->host), "%s", val);
            else if (strcmp(line, "port") == 0) cur->port = atoi(val);
            else if (strcmp(line, "user") == 0) snprintf(cur->user, sizeof(cur->user), "%s", val);
            else if (strcmp(line, "keyfile") == 0) snprintf(cur->keyfile, sizeof(cur->keyfile), "%s", val);
            else if (strcmp(line, "protocol") == 0) cur->protocol = atoi(val);
            else if (strcmp(line, "pinned") == 0) cur->pinned = atoi(val);
            else if (strcmp(line, "use_count") == 0) cur->use_count = atoi(val);
        }
    }
    fclose(f);
    return 0;
}

int favs_save(const FavoriteStore *fs)
{
    if (!fs->filepath[0]) return -1;
    FILE *f = fopen(fs->filepath, "w");
    if (!f) return -1;
    for (int i = 0; i < fs->count; i++) {
        const Favorite *fav = &fs->items[i];
        fprintf(f, "[%s]\nhost=%s\nport=%d\nuser=%s\n", fav->name, fav->host, fav->port, fav->user);
        if (fav->keyfile[0]) fprintf(f, "keyfile=%s\n", fav->keyfile);
        fprintf(f, "protocol=%d\npinned=%d\nuse_count=%d\n\n", fav->protocol, fav->pinned, fav->use_count);
    }
    fclose(f);
    return 0;
}

int favs_add(FavoriteStore *fs, const Favorite *fav)
{
    if (fs->count >= FAV_MAX) return -1;
    fs->items[fs->count++] = *fav;
    return 0;
}

int favs_remove(FavoriteStore *fs, int index)
{
    if (index < 0 || index >= fs->count) return -1;
    memmove(&fs->items[index], &fs->items[index + 1], (fs->count - index - 1) * sizeof(Favorite));
    fs->count--;
    return 0;
}

Favorite *favs_get(FavoriteStore *fs, int index) { return (index >= 0 && index < fs->count) ? &fs->items[index] : NULL; }

Favorite *favs_find_by_name(FavoriteStore *fs, const char *name)
{
    for (int i = 0; i < fs->count; i++)
        if (strcmp(fs->items[i].name, name) == 0) return &fs->items[i];
    return NULL;
}

static int cmp_usage(const void *a, const void *b) { return ((const Favorite *)b)->use_count - ((const Favorite *)a)->use_count; }
static int cmp_name(const void *a, const void *b) { return strcmp(((const Favorite *)a)->name, ((const Favorite *)b)->name); }

int favs_sort_by_usage(FavoriteStore *fs) { qsort(fs->items, fs->count, sizeof(Favorite), cmp_usage); return 0; }
int favs_sort_by_name(FavoriteStore *fs) { qsort(fs->items, fs->count, sizeof(Favorite), cmp_name); return 0; }

int favs_count_with_tag(FavoriteStore *fs, const char *tag)
{
    int n = 0;
    for (int i = 0; i < fs->count; i++)
        for (int j = 0; j < fs->items[i].tag_count; j++)
            if (strcmp(fs->items[i].tags[j], tag) == 0) { n++; break; }
    return n;
}
