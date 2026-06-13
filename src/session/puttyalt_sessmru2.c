/* puttyalt_sessmru2.c - MRU session list v2 with access frequency weighting. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define SM_MAX 48
#define SM_NAME 96
typedef struct {
    char name[SM_NAME];
    uint64_t last_access;
    int access_count;
    int pinned;
} sm_entry;
typedef struct {
    sm_entry entries[SM_MAX];
    int n;
} SessMru2;
void sessmru2_init(SessMru2 *s) {
    if (s) memset(s, 0, sizeof(*s));
}
int sessmru2_touch(SessMru2 *s, const char *name, uint64_t now_ms) {
    if (!s || !name) return -1;
    for (int i = 0; i < s->n; i++) {
        if (strcmp(s->entries[i].name, name) == 0) {
            s->entries[i].last_access = now_ms;
            s->entries[i].access_count++;
            return 0;
        }
    }
    if (s->n >= SM_MAX) {
        int lru = -1;
        uint64_t oldest = UINT64_MAX;
        for (int i = 0; i < s->n; i++) {
            if (!s->entries[i].pinned && s->entries[i].last_access < oldest) {
                oldest = s->entries[i].last_access;
                lru = i;
            }
        }
        if (lru < 0) return -1;
        memmove(&s->entries[lru], &s->entries[lru + 1], sizeof(sm_entry) * (s->n - lru - 1));
        s->n--;
    }
    sm_entry *e = &s->entries[s->n++];
    snprintf(e->name, SM_NAME, "%s", name);
    e->last_access = now_ms;
    e->access_count = 1;
    return 0;
}
int sessmru2_score(const SessMru2 *s, int idx, uint64_t now_ms) {
    if (!s || idx < 0 || idx >= s->n) return 0;
    const sm_entry *e = &s->entries[idx];
    int recency = (int)((now_ms - e->last_access) / 1000);
    int score = e->access_count * 100 - recency / 60;
    if (e->pinned) score += 100000;
    return score;
}
int sessmru2_pin(SessMru2 *s, const char *name, int pin) {
    if (!s || !name) return -1;
    for (int i = 0; i < s->n; i++) {
        if (strcmp(s->entries[i].name, name) == 0) {
            s->entries[i].pinned = pin ? 1 : 0;
            return 0;
        }
    }
    return -1;
}
int sessmru2_count(const SessMru2 *s) {
    return s ? s->n : -1;
}
