/* puttyalt_settingdiff2.c - Diff two setting sets and report changes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define SD_MAX 32
typedef struct { char key[32]; char val[48]; } SdKv;
typedef struct { SdKv kv[SD_MAX]; int n; } SettingSet;
void sd3_init(SettingSet *s) { if (s) s->n = 0; }
int sd3_set(SettingSet *s, const char *key, const char *val) {
    if (!s || s->n >= SD_MAX) return -1;
    strncpy(s->kv[s->n].key, key, 31); s->kv[s->n].key[31] = 0;
    strncpy(s->kv[s->n].val, val, 47); s->kv[s->n].val[47] = 0;
    s->n++;
    return 0;
}
static const char *sd3_lookup(const SettingSet *s, const char *key) {
    for (int i = 0; i < s->n; i++) if (strcmp(s->kv[i].key, key) == 0) return s->kv[i].val;
    return 0;
}
/* Count keys that differ or were added/removed between a and b. */
int sd3_diff_count(const SettingSet *a, const SettingSet *b) {
    if (!a || !b) return 0;
    int changes = 0;
    for (int i = 0; i < a->n; i++) {
        const char *bv = sd3_lookup(b, a->kv[i].key);
        if (!bv || strcmp(bv, a->kv[i].val) != 0) changes++;
    }
    for (int i = 0; i < b->n; i++) if (!sd3_lookup(a, b->kv[i].key)) changes++;
    return changes;
}
