/* puttyalt_pluglife.c - Plugin API v2 lifecycle driver with sandbox grants.
 * MinGW/Windows target. Self-contained, C99. No project headers. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define PL_MAX_ID    64
#define PL_MAX_GRANTS 16
#define PL_GRANT_LEN 32

typedef enum {
    PLIFE_NONE = 0,   /* nothing loaded */
    PLIFE_LOADED,     /* manifest accepted, not active */
    PLIFE_ENABLED,    /* active in sandbox */
    PLIFE_DISABLED    /* loaded but deactivated */
} PLifeState;

typedef struct {
    char  id[PL_MAX_ID];
    uint32_t api_version;          /* must be 2 */
    int   needs_grants;            /* count of required grants */
    char  required[PL_MAX_GRANTS][PL_GRANT_LEN];
} PMManifest;

typedef struct {
    int  count;
    char grant[PL_MAX_GRANTS][PL_GRANT_LEN];
} PGGrantSet;

typedef struct {
    PLifeState state;
    char       id[PL_MAX_ID];
    int        granted;            /* required grants all satisfied */
    uint32_t   transitions;        /* count of successful transitions */
    char       last[96];           /* human-readable status */
} PLifeCtx;

static int pl_has_grant(const PGGrantSet *gs, const char *need) {
    int i;
    if (!gs) return 0;
    for (i = 0; i < gs->count && i < PL_MAX_GRANTS; i++)
        if (strncmp(gs->grant[i], need, PL_GRANT_LEN) == 0) return 1;
    return 0;
}

void plife_init(PLifeCtx *c) {
    if (!c) return;
    memset(c, 0, sizeof(*c));
    c->state = PLIFE_NONE;
    snprintf(c->last, sizeof(c->last), "init");
}

int plife_load(PLifeCtx *c, const PMManifest *m, const PGGrantSet *gs) {
    int i, n;
    if (!c || !m) return -1;
    if (c->state != PLIFE_NONE) {           /* must unload first */
        snprintf(c->last, sizeof(c->last), "load denied: state=%d", c->state);
        return -1;
    }
    if (m->api_version != 2 || m->id[0] == '\0') {
        snprintf(c->last, sizeof(c->last), "load denied: bad manifest");
        return -1;
    }
    n = m->needs_grants;
    if (n < 0 || n > PL_MAX_GRANTS) return -1;
    c->granted = 1;
    for (i = 0; i < n; i++) {
        if (!pl_has_grant(gs, m->required[i])) { c->granted = 0; break; }
    }
    snprintf(c->id, sizeof(c->id), "%s", m->id);
    c->state = PLIFE_LOADED;
    c->transitions++;
    snprintf(c->last, sizeof(c->last), "loaded %s granted=%d", c->id, c->granted);
    return 0;
}

int plife_enable(PLifeCtx *c) {
    if (!c) return -1;
    if (c->state != PLIFE_LOADED && c->state != PLIFE_DISABLED) {
        snprintf(c->last, sizeof(c->last), "enable denied: state=%d", c->state);
        return -1;
    }
    if (!c->granted) {                       /* grant-gated activation */
        snprintf(c->last, sizeof(c->last), "enable denied: missing grants");
        return -1;
    }
    c->state = PLIFE_ENABLED;
    c->transitions++;
    snprintf(c->last, sizeof(c->last), "enabled %s", c->id);
    return 0;
}

int plife_disable(PLifeCtx *c) {
    if (!c) return -1;
    if (c->state != PLIFE_ENABLED) {
        snprintf(c->last, sizeof(c->last), "disable denied: state=%d", c->state);
        return -1;
    }
    c->state = PLIFE_DISABLED;
    c->transitions++;
    snprintf(c->last, sizeof(c->last), "disabled %s", c->id);
    return 0;
}

int plife_unload(PLifeCtx *c) {
    if (!c) return -1;
    if (c->state == PLIFE_NONE) {
        snprintf(c->last, sizeof(c->last), "unload denied: nothing loaded");
        return -1;
    }
    if (c->state == PLIFE_ENABLED) {         /* must disable before unload */
        snprintf(c->last, sizeof(c->last), "unload denied: still enabled");
        return -1;
    }
    c->state = PLIFE_NONE;
    c->granted = 0;
    c->transitions++;
    snprintf(c->last, sizeof(c->last), "unloaded %s", c->id);
    c->id[0] = '\0';
    return 0;
}

PLifeState plife_state(const PLifeCtx *c) {
    return c ? c->state : PLIFE_NONE;
}
