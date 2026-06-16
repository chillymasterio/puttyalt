/* puttyalt_kvstore.c - In-memory key/value string store.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define KVS_CAP 64
#define KVS_KLEN 48
#define KVS_VLEN 128
typedef struct { char key[KVS_KLEN]; char val[KVS_VLEN]; int used; } KvEnt;
typedef struct { KvEnt e[KVS_CAP]; } KvStore;
void kvs_init(KvStore *s) { if (s) for (int i=0;i<KVS_CAP;i++) s->e[i].used=0; }
int kvs_set(KvStore *s, const char *k, const char *v) {
    if (!s || !k || !v) return -1;
    int slot = -1;
    for (int i=0;i<KVS_CAP;i++) {
        if (s->e[i].used && strcmp(s->e[i].key, k)==0) { slot=i; break; }
        if (!s->e[i].used && slot<0) slot=i;
    }
    if (slot<0) return -1;
    strncpy(s->e[slot].key, k, KVS_KLEN-1); s->e[slot].key[KVS_KLEN-1]=0;
    strncpy(s->e[slot].val, v, KVS_VLEN-1); s->e[slot].val[KVS_VLEN-1]=0;
    s->e[slot].used=1;
    return 0;
}
const char *kvs_get(const KvStore *s, const char *k) {
    if (!s||!k) return 0;
    for (int i=0;i<KVS_CAP;i++) if (s->e[i].used && strcmp(s->e[i].key,k)==0) return s->e[i].val;
    return 0;
}
int kvs_del(KvStore *s, const char *k) {
    if (!s||!k) return -1;
    for (int i=0;i<KVS_CAP;i++) if (s->e[i].used && strcmp(s->e[i].key,k)==0) { s->e[i].used=0; return 0; }
    return -1;
}
