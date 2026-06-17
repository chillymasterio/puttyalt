/* puttyalt_sftpmirror.c - Local<->remote directory mirror diff (add/update/delete sets). */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SM_MAX 128
#define SM_PATH 192
typedef struct { char path[SM_PATH]; uint64_t mtime; uint64_t size; } sm_file;
typedef struct { sm_file local[SM_MAX]; int nl; sm_file remote[SM_MAX]; int nr; } SftpMirror;
void sftpmirror_init(SftpMirror *m) { if(m) memset(m,0,sizeof(*m)); }
int sftpmirror_add_local(SftpMirror *m, const char *path, uint64_t mtime, uint64_t size) {
    if(!m||m->nl>=SM_MAX) return -1;
    sm_file *f=&m->local[m->nl++]; snprintf(f->path,SM_PATH,"%s",path); f->mtime=mtime; f->size=size; return 0;
}
int sftpmirror_add_remote(SftpMirror *m, const char *path, uint64_t mtime, uint64_t size) {
    if(!m||m->nr>=SM_MAX) return -1;
    sm_file *f=&m->remote[m->nr++]; snprintf(f->path,SM_PATH,"%s",path); f->mtime=mtime; f->size=size; return 0;
}
static sm_file *sm_find(sm_file *arr, int n, const char *path) {
    for (int i=0;i<n;i++) if (strcmp(arr[i].path,path)==0) return &arr[i];
    return NULL;
}
int sftpmirror_diff(SftpMirror *m, char *upload, int ulen, char *del, int dlen) {
    if(!m) return -1;
    int up=0, dl=0, changes=0;
    for (int i=0;i<m->nl;i++) {
        sm_file *r=sm_find(m->remote,m->nr,m->local[i].path);
        if (!r || r->mtime<m->local[i].mtime || r->size!=m->local[i].size) {
            if (upload && up<ulen) up+=snprintf(upload+up,ulen-up,"%s\n",m->local[i].path);
            changes++;
        }
    }
    for (int i=0;i<m->nr;i++) if (!sm_find(m->local,m->nl,m->remote[i].path)) {
        if (del && dl<dlen) dl+=snprintf(del+dl,dlen-dl,"%s\n",m->remote[i].path);
        changes++;
    }
    return changes;
}
