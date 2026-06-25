/* puttyalt_hashmap2.c - Open-addressing string hash map.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define HM_CAP 128
typedef struct { char key[40]; long value; int used; } HmSlot;
typedef struct { HmSlot slots[HM_CAP]; int count; } HashMap2;
void hm5_init(HashMap2 *m) { if (m) { m->count = 0; for (int i=0;i<HM_CAP;i++) m->slots[i].used = 0; } }
static unsigned hm5_hash(const char *s) {
    unsigned h = 2166136261u;
    for (; *s; s++) { h ^= (unsigned char)*s; h *= 16777619u; }
    return h;
}
int hm5_put(HashMap2 *m, const char *key, long value) {
    if (!m || !key || m->count >= HM_CAP) return -1;
    unsigned h = hm5_hash(key) % HM_CAP;
    for (int i = 0; i < HM_CAP; i++) {
        int idx = (h + i) % HM_CAP;
        if (!m->slots[idx].used || strcmp(m->slots[idx].key, key) == 0) {
            if (!m->slots[idx].used) { strncpy(m->slots[idx].key, key, 39); m->slots[idx].key[39]=0; m->slots[idx].used = 1; m->count++; }
            m->slots[idx].value = value;
            return 0;
        }
    }
    return -1;
}
int hm5_get(const HashMap2 *m, const char *key, long *out) {
    if (!m || !key) return -1;
    unsigned h = hm5_hash(key) % HM_CAP;
    for (int i = 0; i < HM_CAP; i++) {
        int idx = (h + i) % HM_CAP;
        if (!m->slots[idx].used) return -1;
        if (strcmp(m->slots[idx].key, key) == 0) { if (out) *out = m->slots[idx].value; return 0; }
    }
    return -1;
}
