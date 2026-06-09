/* puttyalt_bookmark2.c - Connection bookmarks v2 with folders + icons. */
#include <string.h>
#include <stdio.h>
#define BM_MAX 64
#define BM_NAME 64
typedef struct { char name[BM_NAME]; char target[128]; int folder; int icon; int favorite; } bm_entry;
typedef struct { bm_entry e[BM_MAX]; int n; } Bookmark2;
void bookmark2_init(Bookmark2 *b) { if(b) memset(b,0,sizeof(*b)); }
int bookmark2_add(Bookmark2 *b, const char *name, const char *target, int folder, int icon) {
    if(!b||b->n>=BM_MAX||!name) return -1;
    bm_entry *e=&b->e[b->n]; snprintf(e->name,BM_NAME,"%s",name); snprintf(e->target,128,"%s",target?target:"");
    e->folder=folder; e->icon=icon; return b->n++;
}
void bookmark2_toggle_favorite(Bookmark2 *b, int idx) { if(b&&idx>=0&&idx<b->n) b->e[idx].favorite^=1; }
int bookmark2_in_folder(const Bookmark2 *b, int folder, int *out_idx, int cap) {
    if(!b) return -1; int n=0;
    for (int i=0;i<b->n && n<cap;i++) if (b->e[i].folder==folder) out_idx[n++]=i;
    return n;
}
int bookmark2_favorites(const Bookmark2 *b, int *out_idx, int cap) {
    if(!b) return -1; int n=0;
    for (int i=0;i<b->n && n<cap;i++) if (b->e[i].favorite) out_idx[n++]=i;
    return n;
}
int bookmark2_count(const Bookmark2 *b) { return b?b->n:-1; }
