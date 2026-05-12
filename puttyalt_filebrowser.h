#ifndef PUTTYALT_FILEBROWSER_H
#define PUTTYALT_FILEBROWSER_H

#define FB_MAX_ENTRIES  512
#define FB_MAX_PATH     1024
#define FB_MAX_NAME     256

typedef enum {
    FB_FILE = 0,
    FB_DIR,
    FB_LINK,
    FB_SPECIAL
} FBType;

typedef struct {
    char    name[FB_MAX_NAME];
    FBType  type;
    long    size;
    long    mtime;
    int     permissions;
    char    owner[64];
    int     selected;
} FBEntry;

typedef struct {
    FBEntry entries[FB_MAX_ENTRIES];
    int     count;
    char    current_path[FB_MAX_PATH];
    char    filter[128];
    int     sort_by;        /* 0=name, 1=size, 2=date, 3=type */
    int     sort_asc;
    int     show_hidden;
    int     cursor;
    long    total_size;
} FileBrowser;

void fb_init(FileBrowser *fb);
int  fb_set_path(FileBrowser *fb, const char *path);
int  fb_navigate(FileBrowser *fb, const char *name);
int  fb_go_up(FileBrowser *fb);
int  fb_add_entry(FileBrowser *fb, const char *name, FBType type,
                  long size, long mtime, int perms, const char *owner);
void fb_sort(FileBrowser *fb);
void fb_set_filter(FileBrowser *fb, const char *filter);
int  fb_get_filtered(const FileBrowser *fb, int *indices, int max);
void fb_toggle_select(FileBrowser *fb, int index);
void fb_select_all(FileBrowser *fb);
void fb_deselect_all(FileBrowser *fb);
int  fb_get_selected(const FileBrowser *fb, int *indices, int max);
int  fb_format_size(long size, char *buf, int bufsz);

#endif
