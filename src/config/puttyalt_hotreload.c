/* puttyalt_hotreload.c - Config hot-reload watcher (mtime + content hash). */
#include <string.h>
#include <stdint.h>
typedef struct { uint64_t last_mtime; uint32_t last_hash; int reload_count; int dirty; } HotReload;
void hotreload_init(HotReload *h) { if(h) memset(h,0,sizeof(*h)); }
static uint32_t hr_hash(const char *data, int len) {
    uint32_t h=2166136261u; for (int i=0;i<len;i++) { h^=(uint8_t)data[i]; h*=16777619u; } return h;
}
int hotreload_check(HotReload *h, uint64_t mtime, const char *content, int len) {
    if(!h) return -1;
    if (mtime==h->last_mtime) return 0;
    uint32_t hash=hr_hash(content?content:"",content?len:0);
    h->last_mtime=mtime;
    if (hash==h->last_hash) return 0;
    /* mtime changed but content same */
    h->last_hash=hash; h->dirty=1; h->reload_count++; return 1;
}
void hotreload_consumed(HotReload *h) { if(h) h->dirty=0; }
int hotreload_count(const HotReload *h) { return h?h->reload_count:-1; }
