/* puttyalt_pgrant.c - Plugin API v2 capability/permission grant enforcement.
 * Maps manifest-declared capabilities to a granted permission set and
 * enforces per-plugin capability checks at call time. Self-contained. */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Capabilities a plugin may request/be granted. PG_CAP_COUNT bounds arrays. */
typedef enum {
    PG_CAP_NET = 0,     /* outbound network access            */
    PG_CAP_FS_READ,     /* read host filesystem               */
    PG_CAP_FS_WRITE,    /* write host filesystem              */
    PG_CAP_CLIPBOARD,   /* read/write clipboard               */
    PG_CAP_TERM_WRITE,  /* inject data into terminal          */
    PG_CAP_CONFIG,      /* read/modify session config         */
    PG_CAP_EXEC,        /* spawn external processes           */
    PG_CAP_COUNT        /* sentinel: number of capabilities   */
} PGCapability;

#define PG_NAME_MAX 32

/* One manifest-declared capability entry. */
typedef struct {
    PGCapability cap;
    int requested;          /* nonzero if the manifest asked for this cap */
} PMCapReq;

/* Parsed plugin manifest. */
typedef struct {
    char name[PG_NAME_MAX];
    PMCapReq caps[PG_CAP_COUNT];
    int n_caps;             /* number of valid entries in caps[] */
} PMManifest;

/* Resolved permission set for a single loaded plugin. */
typedef struct {
    char owner[PG_NAME_MAX];
    uint8_t granted[PG_CAP_COUNT];  /* 1 = granted, 0 = denied/revoked */
    int initialized;
} PGGrantSet;

static const char *pg_cap_name(PGCapability c)
{
    static const char *names[PG_CAP_COUNT] = {
        "net", "fs_read", "fs_write", "clipboard",
        "term_write", "config", "exec"
    };
    if ((int)c < 0 || (int)c >= PG_CAP_COUNT)
        return "?";
    return names[c];
}

void pgrant_init(PGGrantSet *gs)
{
    if (!gs)
        return;
    memset(gs, 0, sizeof(*gs));
    gs->initialized = 1;
}

int pgrant_from_manifest(PGGrantSet *gs, const PMManifest *m)
{
    int i, n;
    if (!gs || !m || !gs->initialized)
        return -1;
    if (m->n_caps < 0 || m->n_caps > PG_CAP_COUNT)
        return -1;

    memset(gs->granted, 0, sizeof(gs->granted));
    n = (int)strlen(m->name);
    if (n >= PG_NAME_MAX)
        n = PG_NAME_MAX - 1;
    memcpy(gs->owner, m->name, (size_t)n);
    gs->owner[n] = '\0';

    for (i = 0; i < m->n_caps; i++) {
        PGCapability c = m->caps[i].cap;
        if ((int)c < 0 || (int)c >= PG_CAP_COUNT)
            return -1;
        if (m->caps[i].requested)
            gs->granted[c] = 1;
    }
    return 0;
}

int pgrant_check(const PGGrantSet *gs, PGCapability cap)
{
    if (!gs || !gs->initialized)
        return -1;
    if ((int)cap < 0 || (int)cap >= PG_CAP_COUNT)
        return -1;
    return gs->granted[cap] ? 0 : -1;
}

int pgrant_revoke(PGGrantSet *gs, PGCapability cap)
{
    if (!gs || !gs->initialized)
        return -1;
    if ((int)cap < 0 || (int)cap >= PG_CAP_COUNT)
        return -1;
    gs->granted[cap] = 0;
    return 0;
}

int pgrant_describe(const PGGrantSet *gs, char *buf, int len)
{
    int i, off, first = 1;
    if (!gs || !gs->initialized || !buf || len <= 0)
        return -1;

    off = snprintf(buf, (size_t)len, "%s:[", gs->owner);
    if (off < 0 || off >= len)
        return -1;

    for (i = 0; i < PG_CAP_COUNT; i++) {
        if (!gs->granted[i])
            continue;
        off += snprintf(buf + off, (size_t)(len - off), "%s%s",
                        first ? "" : ",", pg_cap_name((PGCapability)i));
        if (off < 0 || off >= len)
            return -1;
        first = 0;
    }

    off += snprintf(buf + off, (size_t)(len - off), "]");
    if (off < 0 || off >= len)
        return -1;
    return off;
}
