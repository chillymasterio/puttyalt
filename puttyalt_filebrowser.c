#include "puttyalt_filebrowser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fb_init(FileBrowser *fb)
{
    memset(fb, 0, sizeof(*fb));
    fb->sort_asc = 1;
    fb->show_hidden = 0;
    snprintf(fb->current_path, FB_MAX_PATH, "/");
}

int fb_set_path(FileBrowser *fb, const char *path)
{
    snprintf(fb->current_path, FB_MAX_PATH, "%s", path);
    fb->count = 0;
    fb->cursor = 0;
    fb->total_size = 0;
    return 0;
}

int fb_navigate(FileBrowser *fb, const char *name)
{
    char newpath[FB_MAX_PATH];
    size_t len = strlen(fb->current_path);
    if (len > 1 && fb->current_path[len-1] != '/')
        snprintf(newpath, FB_MAX_PATH, "%s/%s", fb->current_path, name);
    else
        snprintf(newpath, FB_MAX_PATH, "%s%s", fb->current_path, name);
    return fb_set_path(fb, newpath);
}

int fb_go_up(FileBrowser *fb)
{
    char *last = strrchr(fb->current_path, '/');
    if (!last || last == fb->current_path) {
        fb->current_path[0] = '/';
        fb->current_path[1] = '\0';
    } else {
        *last = '\0';
    }
    fb->count = 0;
    fb->cursor = 0;
    return 0;
}

int fb_add_entry(FileBrowser *fb, const char *name, FBType type,
                 long size, long mtime, int perms, const char *owner)
{
    if (fb->count >= FB_MAX_ENTRIES) return -1;
    FBEntry *e = &fb->entries[fb->count];
    memset(e, 0, sizeof(*e));
    snprintf(e->name, FB_MAX_NAME, "%s", name);
    e->type = type;
    e->size = size;
    e->mtime = mtime;
    e->permissions = perms;
    if (owner) snprintf(e->owner, sizeof(e->owner), "%s", owner);
    fb->total_size += size;
    return fb->count++;
}

static int cmp_name(const void *a, const void *b)
{
    return strcmp(((const FBEntry *)a)->name, ((const FBEntry *)b)->name);
}
static int cmp_size(const void *a, const void *b)
{
    long da = ((const FBEntry *)a)->size, db = ((const FBEntry *)b)->size;
    return da < db ? -1 : da > db ? 1 : 0;
}
static int cmp_date(const void *a, const void *b)
{
    long da = ((const FBEntry *)a)->mtime, db = ((const FBEntry *)b)->mtime;
    return da < db ? -1 : da > db ? 1 : 0;
}

void fb_sort(FileBrowser *fb)
{
    int (*cmpfn)(const void*, const void*) = cmp_name;
    if (fb->sort_by == 1) cmpfn = cmp_size;
    else if (fb->sort_by == 2) cmpfn = cmp_date;

    /* Separate dirs and files: dirs first */
    FBEntry dirs[FB_MAX_ENTRIES], files[FB_MAX_ENTRIES];
    int dc = 0, fc = 0;
    for (int i = 0; i < fb->count; i++) {
        if (fb->entries[i].type == FB_DIR) dirs[dc++] = fb->entries[i];
        else files[fc++] = fb->entries[i];
    }
    if (dc > 1) qsort(dirs, dc, sizeof(FBEntry), cmpfn);
    if (fc > 1) qsort(files, fc, sizeof(FBEntry), cmpfn);
    int idx = 0;
    for (int i = 0; i < dc; i++) fb->entries[idx++] = dirs[fb->sort_asc ? i : dc-1-i];
    for (int i = 0; i < fc; i++) fb->entries[idx++] = files[fb->sort_asc ? i : fc-1-i];
}

void fb_set_filter(FileBrowser *fb, const char *filter)
{
    if (filter) snprintf(fb->filter, sizeof(fb->filter), "%s", filter);
    else fb->filter[0] = '\0';
}

int fb_get_filtered(const FileBrowser *fb, int *indices, int max)
{
    int n = 0;
    for (int i = 0; i < fb->count && n < max; i++) {
        if (!fb->show_hidden && fb->entries[i].name[0] == '.') continue;
        if (fb->filter[0] && !strstr(fb->entries[i].name, fb->filter)) continue;
        indices[n++] = i;
    }
    return n;
}

void fb_toggle_select(FileBrowser *fb, int index)
{
    if (index >= 0 && index < fb->count)
        fb->entries[index].selected = !fb->entries[index].selected;
}

void fb_select_all(FileBrowser *fb)
{
    for (int i = 0; i < fb->count; i++) fb->entries[i].selected = 1;
}

void fb_deselect_all(FileBrowser *fb)
{
    for (int i = 0; i < fb->count; i++) fb->entries[i].selected = 0;
}

int fb_get_selected(const FileBrowser *fb, int *indices, int max)
{
    int n = 0;
    for (int i = 0; i < fb->count && n < max; i++)
        if (fb->entries[i].selected) indices[n++] = i;
    return n;
}

int fb_format_size(long size, char *buf, int bufsz)
{
    if (size < 1024) return snprintf(buf, bufsz, "%ld B", size);
    if (size < 1024*1024) return snprintf(buf, bufsz, "%.1f KB", size/1024.0);
    if (size < 1024*1024*1024) return snprintf(buf, bufsz, "%.1f MB", size/(1024.0*1024));
    return snprintf(buf, bufsz, "%.2f GB", size/(1024.0*1024*1024));
}
