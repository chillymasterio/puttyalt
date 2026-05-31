/* puttyalt_sharetier.c - permission tiers for shared sessions (PuttyAlt).
 *
 * Defines viewer/commenter/editor/owner capability tiers and resolves whether
 * a peer may view, annotate, send input, kick, or grant. The host can set a
 * max-tier cap (re-clamping existing grants), lock the session, and promote or
 * demote individual peers. Self-contained: all types are defined below.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SHARETIER_MAX_PEERS 32
#define SHARETIER_ID_LEN    48

/* Capability tiers, ordered by increasing privilege. */
typedef enum {
    SHARETIER_VIEWER = 0,   /* may view output */
    SHARETIER_COMMENTER,    /* may view + annotate */
    SHARETIER_EDITOR,       /* may view + annotate + send input */
    SHARETIER_OWNER         /* full control: kick + grant */
} sharetier_tier_t;

/* Capabilities resolvable per peer. */
typedef enum {
    SHARECAP_VIEW = 0,
    SHARECAP_ANNOTATE,
    SHARECAP_INPUT,
    SHARECAP_KICK,
    SHARECAP_GRANT
} sharetier_cap_t;

typedef struct {
    char             id[SHARETIER_ID_LEN];
    sharetier_tier_t tier;
    uint64_t         granted_ms;
    int              used;
} sharetier_peer_t;

typedef struct {
    sharetier_peer_t peers[SHARETIER_MAX_PEERS];
    sharetier_tier_t max_tier;   /* host-imposed cap */
    int              locked;     /* 1 = no new grants */
    int              count;
} sharetier_table_t;

static sharetier_tier_t sharetier_clamp(sharetier_tier_t t, sharetier_tier_t cap)
{
    if (t < SHARETIER_VIEWER) t = SHARETIER_VIEWER;
    if (t > SHARETIER_OWNER)  t = SHARETIER_OWNER;
    return (t > cap) ? cap : t;
}

void sharetier_init(sharetier_table_t *t)
{
    if (!t) return;
    memset(t, 0, sizeof(*t));
    t->max_tier = SHARETIER_OWNER;
    t->locked = 0;
    t->count = 0;
}

/* Set cap and re-clamp every existing grant. Returns 0 ok, -1 on bad args. */
int sharetier_set_max(sharetier_table_t *t, sharetier_tier_t max_tier)
{
    int i;
    if (!t || max_tier < SHARETIER_VIEWER || max_tier > SHARETIER_OWNER)
        return -1;
    t->max_tier = max_tier;
    for (i = 0; i < SHARETIER_MAX_PEERS; i++)
        if (t->peers[i].used)
            t->peers[i].tier = sharetier_clamp(t->peers[i].tier, max_tier);
    return 0;
}

int sharetier_set_locked(sharetier_table_t *t, int locked)
{
    if (!t) return -1;
    t->locked = locked ? 1 : 0;
    return 0;
}

static sharetier_peer_t *sharetier_find(sharetier_table_t *t, const char *id)
{
    int i;
    for (i = 0; i < SHARETIER_MAX_PEERS; i++)
        if (t->peers[i].used &&
            strncmp(t->peers[i].id, id, SHARETIER_ID_LEN) == 0)
            return &t->peers[i];
    return NULL;
}

/* Grant or promote/demote a peer. Returns 0 ok, -1 on error/full/locked. */
int sharetier_grant(sharetier_table_t *t, const char *id,
                    sharetier_tier_t tier, uint64_t now_ms)
{
    sharetier_peer_t *p;
    int i;
    if (!t || !id || id[0] == '\0')
        return -1;
    tier = sharetier_clamp(tier, t->max_tier);
    p = sharetier_find(t, id);
    if (p) {                        /* existing peer: re-tier even if locked */
        p->tier = tier;
        p->granted_ms = now_ms;
        return 0;
    }
    if (t->locked)
        return -1;
    for (i = 0; i < SHARETIER_MAX_PEERS; i++) {
        if (!t->peers[i].used) {
            p = &t->peers[i];
            memset(p, 0, sizeof(*p));
            snprintf(p->id, SHARETIER_ID_LEN, "%s", id);
            p->tier = tier;
            p->granted_ms = now_ms;
            p->used = 1;
            t->count++;
            return 0;
        }
    }
    return -1;                       /* table full */
}

int sharetier_revoke(sharetier_table_t *t, const char *id)
{
    sharetier_peer_t *p;
    if (!t || !id) return -1;
    p = sharetier_find(t, id);
    if (!p) return -1;
    memset(p, 0, sizeof(*p));
    if (t->count > 0) t->count--;
    return 0;
}

/* Returns the peer's tier (>=0), or -1 if unknown. */
int sharetier_get(const sharetier_table_t *t, const char *id)
{
    int i;
    if (!t || !id) return -1;
    for (i = 0; i < SHARETIER_MAX_PEERS; i++)
        if (t->peers[i].used &&
            strncmp(t->peers[i].id, id, SHARETIER_ID_LEN) == 0)
            return (int)t->peers[i].tier;
    return -1;
}

/* Minimum tier required to exercise a capability. */
sharetier_tier_t sharetier_min_for(sharetier_cap_t cap)
{
    switch (cap) {
        case SHARECAP_VIEW:     return SHARETIER_VIEWER;
        case SHARECAP_ANNOTATE: return SHARETIER_COMMENTER;
        case SHARECAP_INPUT:    return SHARETIER_EDITOR;
        case SHARECAP_KICK:     return SHARETIER_OWNER;
        case SHARECAP_GRANT:    return SHARETIER_OWNER;
        default:                return SHARETIER_OWNER;
    }
}

/* Returns 1 if the peer may perform cap, 0 otherwise. */
int sharetier_can(const sharetier_table_t *t, const char *id,
                  sharetier_cap_t cap)
{
    int tier = sharetier_get(t, id);
    if (tier < 0) return 0;
    return (tier >= (int)sharetier_min_for(cap)) ? 1 : 0;
}

const char *sharetier_name(sharetier_tier_t tier)
{
    switch (tier) {
        case SHARETIER_VIEWER:    return "viewer";
        case SHARETIER_COMMENTER: return "commenter";
        case SHARETIER_EDITOR:    return "editor";
        case SHARETIER_OWNER:     return "owner";
        default:                  return "unknown";
    }
}
