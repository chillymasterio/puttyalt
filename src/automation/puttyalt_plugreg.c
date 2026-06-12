/* puttyalt_plugreg.c - Plugin API v2 sandbox: static registry of loaded plugins.
 * Indexes plugin v2 entries by id and name, tying manifest, grants, and
 * lifecycle context together. Self-contained C99, no project headers. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define PREG_MAX        64      /* maximum simultaneously loaded plugins */
#define PREG_NAME_LEN   48      /* plugin / manifest name buffer length  */
#define PREG_VER_LEN    16      /* version string buffer length          */

/* Manifest describing a plugin as declared by its package. */
typedef struct {
    char     name[PREG_NAME_LEN];
    char     version[PREG_VER_LEN];
    uint32_t api_level;         /* must be 2 for Plugin API v2 */
} PMManifest;

/* Capability grant bitset negotiated by the sandbox at load time. */
typedef struct {
    uint32_t caps;              /* OR of granted capability bits */
} PGGrantSet;

/* Lifecycle state of a registered entry. */
typedef enum {
    PREG_STATE_FREE = 0,
    PREG_STATE_LOADED,
    PREG_STATE_ACTIVE
} PRegState;

/* One registry slot. */
typedef struct {
    int        id;              /* unique non-negative id, -1 when unused */
    PMManifest manifest;
    PGGrantSet grants;
    PRegState  state;
} PRegEntry;

/* The registry itself: fixed-size backing store plus an id allocator. */
typedef struct {
    PRegEntry entries[PREG_MAX];
    int       count;            /* number of occupied slots */
    int       next_id;          /* monotonically increasing id source */
} PRegistry;

/* Forward declarations (pregistry_add consults pregistry_find). */
void pregistry_init(PRegistry *r);
int  pregistry_add(PRegistry *r, const PMManifest *m, const PGGrantSet *gs);
int  pregistry_find(const PRegistry *r, const char *name);
int  pregistry_remove(PRegistry *r, int id);
int  pregistry_list(const PRegistry *r, char *buf, int len);

void pregistry_init(PRegistry *r)
{
    int i;
    if (!r) return;
    memset(r, 0, sizeof(*r));
    for (i = 0; i < PREG_MAX; i++) {
        r->entries[i].id = -1;
        r->entries[i].state = PREG_STATE_FREE;
    }
    r->count = 0;
    r->next_id = 1;
}

int pregistry_add(PRegistry *r, const PMManifest *m, const PGGrantSet *gs)
{
    int i;
    if (!r || !m || !gs) return -1;
    if (m->api_level != 2u) return -1;
    /* Plugin API v2 only */
    if (m->name[0] == '\0') return -1;
    if (r->count >= PREG_MAX) return -1;
    if (pregistry_find(r, m->name) >= 0) return -1;   /* names are unique */

    for (i = 0; i < PREG_MAX; i++) {
        if (r->entries[i].state == PREG_STATE_FREE) {
            PRegEntry *e = &r->entries[i];
            e->manifest = *m;
            e->manifest.name[PREG_NAME_LEN - 1] = '\0';
            e->manifest.version[PREG_VER_LEN - 1] = '\0';
            e->grants = *gs;
            e->state = PREG_STATE_LOADED;
            e->id = r->next_id++;
            if (r->next_id < 1) r->next_id = 1;
            r->count++;
            return e->id;
        }
    }
    return -1;
}

int pregistry_find(const PRegistry *r, const char *name)
{
    int i;
    if (!r || !name) return -1;
    for (i = 0; i < PREG_MAX; i++) {
        if (r->entries[i].state != PREG_STATE_FREE &&
            strncmp(r->entries[i].manifest.name, name, PREG_NAME_LEN) == 0)
            return r->entries[i].id;
    }
    return -1;
}

int pregistry_remove(PRegistry *r, int id)
{
    int i;
    if (!r || id < 0) return -1;
    for (i = 0; i < PREG_MAX; i++) {
        if (r->entries[i].state != PREG_STATE_FREE && r->entries[i].id == id) {
            memset(&r->entries[i], 0, sizeof(r->entries[i]));
            r->entries[i].id = -1;
            r->entries[i].state = PREG_STATE_FREE;
            if (r->count > 0) r->count--;
            return 0;
        }
    }
    return -1;
}

int pregistry_list(const PRegistry *r, char *buf, int len)
{
    int i, used = 0, n;
    if (!r || !buf || len <= 0) return -1;
    buf[0] = '\0';
    for (i = 0; i < PREG_MAX; i++) {
        const PRegEntry *e = &r->entries[i];
        if (e->state == PREG_STATE_FREE) continue;
        n = snprintf(buf + used, (size_t)(len - used),
                     "%d %s %s caps=0x%08lx st=%d\n",
                     e->id, e->manifest.name, e->manifest.version,
                     (unsigned long)e->grants.caps, (int)e->state);
        if (n < 0) return -1;
        if (n >= len - used) {              /* truncated: stop cleanly */
            buf[len - 1] = '\0';
            return used;
        }
        used += n;
    }
    return used;
}
