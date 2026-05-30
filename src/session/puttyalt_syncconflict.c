/* puttyalt_syncconflict.c - sync conflict resolution for PuttyAlt sessions.
 * Detects divergence between local/remote synced records and resolves via
 * last-writer-wins plus field-level three-way merge against a common base. */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define SC_NFIELDS   5
#define SC_FLDLEN    64

/* Field indices into the value arrays. */
enum { SC_HOST = 0, SC_USER, SC_PORT, SC_THEME, SC_KEYS };

typedef enum {
    SYNC_POLICY_LWW = 0,   /* last-writer-wins by timestamp */
    SYNC_POLICY_LOCAL,     /* prefer local on every conflict */
    SYNC_POLICY_REMOTE,    /* prefer remote on every conflict */
    SYNC_POLICY_MERGE      /* three-way field merge against base */
} SyncResolution;

typedef struct {
    char     fields[SC_NFIELDS][SC_FLDLEN];
    uint64_t mtime_ms;     /* last-write time of this record */
} SyncRecord;

typedef struct {
    SyncRecord local;
    SyncRecord remote;
    SyncRecord base;
    int        have_base;
    int        conflict[SC_NFIELDS]; /* 1 if local/remote differ */
    int        nconflicts;
    int        detected;
} SyncConflictCtx;

static const char *sc_field_name(int i)
{
    static const char *n[SC_NFIELDS] = { "host", "user", "port", "theme", "keys" };
    return (i >= 0 && i < SC_NFIELDS) ? n[i] : "?";
}

void synccon_init(SyncConflictCtx *ctx)
{
    if (ctx) memset(ctx, 0, sizeof(*ctx));
}

int synccon_detect(SyncConflictCtx *ctx, const SyncRecord *local,
                   const SyncRecord *remote, const SyncRecord *base)
{
    int i;
    if (!ctx || !local || !remote) return -1;
    ctx->local = *local;
    ctx->remote = *remote;
    ctx->have_base = (base != NULL);
    if (base) ctx->base = *base;
    ctx->nconflicts = 0;
    for (i = 0; i < SC_NFIELDS; i++) {
        ctx->conflict[i] = (strncmp(local->fields[i], remote->fields[i],
                                    SC_FLDLEN) != 0) ? 1 : 0;
        if (ctx->conflict[i]) ctx->nconflicts++;
    }
    ctx->detected = 1;
    return ctx->nconflicts;
}

static void sc_copyfield(char *dst, const char *src)
{
    size_t n = strnlen(src, SC_FLDLEN - 1);
    memcpy(dst, src, n);
    dst[n] = '\0';
}

int synccon_resolve(SyncConflictCtx *ctx, SyncResolution policy, SyncRecord *out)
{
    int i;
    if (!ctx || !out || !ctx->detected) return -1;
    memset(out, 0, sizeof(*out));
    out->mtime_ms = (ctx->local.mtime_ms > ctx->remote.mtime_ms)
                        ? ctx->local.mtime_ms : ctx->remote.mtime_ms;
    for (i = 0; i < SC_NFIELDS; i++) {
        const char *pick = ctx->local.fields[i];
        if (ctx->conflict[i]) {
            switch (policy) {
            case SYNC_POLICY_LOCAL:
                pick = ctx->local.fields[i]; break;
            case SYNC_POLICY_REMOTE:
                pick = ctx->remote.fields[i]; break;
            case SYNC_POLICY_MERGE:
                if (ctx->have_base) {
                    int lchg = strncmp(ctx->base.fields[i],
                                       ctx->local.fields[i], SC_FLDLEN) != 0;
                    int rchg = strncmp(ctx->base.fields[i],
                                       ctx->remote.fields[i], SC_FLDLEN) != 0;
                    if (lchg && !rchg)      pick = ctx->local.fields[i];
                    else if (!lchg && rchg) pick = ctx->remote.fields[i];
                    else pick = (ctx->local.mtime_ms >= ctx->remote.mtime_ms)
                                    ? ctx->local.fields[i]
                                    : ctx->remote.fields[i];
                    break;
                }
                /* no base: fall through to LWW */
                /* FALLTHROUGH */
            case SYNC_POLICY_LWW:
            default:
                pick = (ctx->local.mtime_ms >= ctx->remote.mtime_ms)
                           ? ctx->local.fields[i] : ctx->remote.fields[i];
                break;
            }
        }
        sc_copyfield(out->fields[i], pick);
    }
    return 0;
}

int synccon_list_fields(SyncConflictCtx *ctx, char *out, size_t outcap)
{
    int i, count = 0;
    size_t used = 0;
    if (!ctx || !out || outcap == 0 || !ctx->detected) return -1;
    out[0] = '\0';
    for (i = 0; i < SC_NFIELDS; i++) {
        int w;
        if (!ctx->conflict[i]) continue;
        w = snprintf(out + used, outcap - used, "%s%s",
                     count ? "," : "", sc_field_name(i));
        if (w < 0 || (size_t)w >= outcap - used) return -1;
        used += (size_t)w;
        count++;
    }
    return count;
}

const char *synccon_policy_name(SyncResolution policy)
{
    switch (policy) {
    case SYNC_POLICY_LWW:    return "last-writer-wins";
    case SYNC_POLICY_LOCAL:  return "prefer-local";
    case SYNC_POLICY_REMOTE: return "prefer-remote";
    case SYNC_POLICY_MERGE:  return "three-way-merge";
    default:                 return "unknown";
    }
}
