/* puttyalt_filesync2.c - Bidirectional file sync conflict detector (rsync-lite). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define FS_MAX 96
#define FS_PATH 160
enum fs_action { FS_SKIP=0, FS_PUSH=1, FS_PULL=2, FS_CONFLICT=3 };
typedef struct { char path[FS_PATH]; uint64_t lmtime, rmtime, bmtime; uint32_t lhash, rhash; } fs_entry;
typedef struct { fs_entry e[FS_MAX]; int n; } FileSync2;
void filesync2_init(FileSync2 *f) { if(f) memset(f,0,sizeof(*f)); }
int filesync2_add(FileSync2 *f, const char *path, uint64_t lmtime, uint64_t rmtime, uint64_t bmtime, uint32_t lhash, uint32_t rhash) {
    if(!f||f->n>=FS_MAX||!path) return -1;
    fs_entry *e=&f->e[f->n++]; snprintf(e->path,FS_PATH,"%s",path);
    e->lmtime=lmtime; e->rmtime=rmtime; e->bmtime=bmtime; e->lhash=lhash; e->rhash=rhash; return 0;
}
int filesync2_action(const FileSync2 *f, int idx) {
    if(!f||idx<0||idx>=f->n) return FS_SKIP;
    const fs_entry *e=&f->e[idx];
    if (e->lhash==e->rhash) return FS_SKIP;
    int local_changed = e->lmtime>e->bmtime || e->lhash!=0;
    int remote_changed = e->rmtime>e->bmtime || e->rhash!=0;
    if (local_changed && remote_changed) {
        if (e->lmtime>e->rmtime) return FS_PUSH;
        if (e->rmtime>e->lmtime) return FS_PULL;
        return FS_CONFLICT;
    }
    if (local_changed) return FS_PUSH;
    if (remote_changed) return FS_PULL;
    return FS_SKIP;
}
int filesync2_count_conflicts(const FileSync2 *f) {
    if(!f) return -1; int c=0; for(int i=0;i<f->n;i++) if(filesync2_action(f,i)==FS_CONFLICT)c++; return c;
}
int filesync2_count(const FileSync2 *f) { return f?f->n:-1; }
