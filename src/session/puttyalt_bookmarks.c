#include <string.h>
#include <stdio.h>
#define MAX_BOOKMARKS 256
#define MAX_FOLDERS 32
typedef struct { char name[128]; char host[256]; int port; char folder[64]; int pinned; } Bookmark;
typedef struct { char name[64]; int count; } BookmarkFolder;
static Bookmark g_bm[MAX_BOOKMARKS]; static int g_bm_count = 0;
static BookmarkFolder g_folders[MAX_FOLDERS]; static int g_folder_count = 0;
int bookmark_add(const char *name, const char *host, int port, const char *folder) {
    if (g_bm_count >= MAX_BOOKMARKS) return -1;
    Bookmark *b = &g_bm[g_bm_count]; memset(b, 0, sizeof(*b));
    snprintf(b->name, 128, "%s", name); snprintf(b->host, 256, "%s", host);
    b->port = port > 0 ? port : 22;
    if (folder) snprintf(b->folder, 64, "%s", folder);
    /* Auto-create folder */
    if (folder && folder[0]) {
        int found = 0;
        for (int i=0;i<g_folder_count;i++) if (strcmp(g_folders[i].name,folder)==0) { g_folders[i].count++; found=1; break; }
        if (!found && g_folder_count < MAX_FOLDERS) { snprintf(g_folders[g_folder_count].name,64,"%s",folder); g_folders[g_folder_count].count=1; g_folder_count++; }
    }
    return g_bm_count++;
}
int bookmark_pin(int idx) { if (idx<0||idx>=g_bm_count) return -1; g_bm[idx].pinned=!g_bm[idx].pinned; return g_bm[idx].pinned; }
int bookmark_find(const char *query, int results[], int max) {
    int n=0;
    for (int i=0;i<g_bm_count&&n<max;i++)
        if (strstr(g_bm[i].name,query)||strstr(g_bm[i].host,query)) results[n++]=i;
    return n;
}
int bookmark_count(void) { return g_bm_count; }
int bookmark_folder_count(void) { return g_folder_count; }
