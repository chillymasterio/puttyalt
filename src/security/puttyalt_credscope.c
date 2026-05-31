/* puttyalt_credscope.c - Per-session credential scoping for PuttyAlt.
 * Binds credentials to a session scope so a credential leased to one
 * session cannot be reused by another. Self-contained C99 module.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CREDSCOPE_MAX_LEASES 64
#define CREDSCOPE_ID_LEN     48

enum credscope_flag {
    CREDSCOPE_F_NONE   = 0,
    CREDSCOPE_F_READ   = 1u << 0,
    CREDSCOPE_F_WRITE  = 1u << 1,
    CREDSCOPE_F_FORWARD= 1u << 2,
    CREDSCOPE_F_ALL    = (1u << 0) | (1u << 1) | (1u << 2)
};

enum credscope_err {
    CREDSCOPE_OK = 0,
    CREDSCOPE_E_INVAL = -1,
    CREDSCOPE_E_FULL  = -2,
    CREDSCOPE_E_NOENT = -3,
    CREDSCOPE_E_DENIED= -4
};

typedef struct credscope_lease {
    int      used;
    char     session_id[CREDSCOPE_ID_LEN];
    char     cred_ref[CREDSCOPE_ID_LEN];
    uint32_t flags;
    uint64_t expiry_ms;
} credscope_lease;

typedef struct credscope_table {
    credscope_lease leases[CREDSCOPE_MAX_LEASES];
    int             count;
} credscope_table;

static credscope_table g_credscope;

static int credscope_copy_id(char *dst, const char *src)
{
    size_t n;
    if (!dst || !src || src[0] == '\0')
        return CREDSCOPE_E_INVAL;
    n = strlen(src);
    if (n >= CREDSCOPE_ID_LEN)
        return CREDSCOPE_E_INVAL;
    memcpy(dst, src, n + 1);
    return CREDSCOPE_OK;
}

void credscope_init(void)
{
    memset(&g_credscope, 0, sizeof(g_credscope));
}

int credscope_grant(const char *session_id, const char *cred_ref,
                    uint32_t flags, uint64_t expiry_ms)
{
    int i, slot = -1;
    if (!session_id || !cred_ref || (flags & ~CREDSCOPE_F_ALL) != 0)
        return CREDSCOPE_E_INVAL;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        if (!g_credscope.leases[i].used) {
            if (slot < 0) slot = i;
        } else if (strcmp(g_credscope.leases[i].session_id, session_id) == 0 &&
                   strcmp(g_credscope.leases[i].cred_ref, cred_ref) == 0) {
            slot = i;
            break;
        }
    }
    if (slot < 0)
        return CREDSCOPE_E_FULL;
    if (credscope_copy_id(g_credscope.leases[slot].session_id, session_id) != 0 ||
        credscope_copy_id(g_credscope.leases[slot].cred_ref, cred_ref) != 0)
        return CREDSCOPE_E_INVAL;
    if (!g_credscope.leases[slot].used) {
        g_credscope.leases[slot].used = 1;
        g_credscope.count++;
    }
    g_credscope.leases[slot].flags = flags;
    g_credscope.leases[slot].expiry_ms = expiry_ms;
    return CREDSCOPE_OK;
}

int credscope_revoke(const char *session_id, const char *cred_ref)
{
    int i;
    if (!session_id || !cred_ref)
        return CREDSCOPE_E_INVAL;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        if (g_credscope.leases[i].used &&
            strcmp(g_credscope.leases[i].session_id, session_id) == 0 &&
            strcmp(g_credscope.leases[i].cred_ref, cred_ref) == 0) {
            memset(&g_credscope.leases[i], 0, sizeof(g_credscope.leases[i]));
            g_credscope.count--;
            return CREDSCOPE_OK;
        }
    }
    return CREDSCOPE_E_NOENT;
}

int credscope_check(const char *session_id, const char *cred_ref,
                    uint32_t want_flags, uint64_t now_ms)
{
    int i;
    if (!session_id || !cred_ref)
        return CREDSCOPE_E_INVAL;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        credscope_lease *l = &g_credscope.leases[i];
        if (!l->used || strcmp(l->cred_ref, cred_ref) != 0)
            continue;
        if (strcmp(l->session_id, session_id) != 0)
            return CREDSCOPE_E_DENIED; /* leased to another session */
        if (l->expiry_ms != 0 && now_ms >= l->expiry_ms)
            return CREDSCOPE_E_DENIED;
        if ((l->flags & want_flags) != want_flags)
            return CREDSCOPE_E_DENIED;
        return CREDSCOPE_OK;
    }
    return CREDSCOPE_E_NOENT;
}

int credscope_bind_session(const char *session_id, const char *cred_ref,
                           uint32_t flags, uint64_t expiry_ms)
{
    return credscope_grant(session_id, cred_ref, flags, expiry_ms);
}

int credscope_release_session(const char *session_id)
{
    int i, n = 0;
    if (!session_id)
        return CREDSCOPE_E_INVAL;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        if (g_credscope.leases[i].used &&
            strcmp(g_credscope.leases[i].session_id, session_id) == 0) {
            memset(&g_credscope.leases[i], 0, sizeof(g_credscope.leases[i]));
            g_credscope.count--;
            n++;
        }
    }
    return n;
}

int credscope_reap_expired(uint64_t now_ms)
{
    int i, n = 0;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        credscope_lease *l = &g_credscope.leases[i];
        if (l->used && l->expiry_ms != 0 && now_ms >= l->expiry_ms) {
            memset(l, 0, sizeof(*l));
            g_credscope.count--;
            n++;
        }
    }
    return n;
}

const char *credscope_flags_name(uint32_t flags)
{
    switch (flags & CREDSCOPE_F_ALL) {
        case CREDSCOPE_F_NONE:  return "none";
        case CREDSCOPE_F_READ:  return "read";
        case CREDSCOPE_F_WRITE: return "write";
        case CREDSCOPE_F_FORWARD: return "forward";
        case CREDSCOPE_F_ALL:   return "all";
        default:                return "mixed";
    }
}

int credscope_describe(const char *session_id, const char *cred_ref,
                       char *buf, size_t buflen)
{
    int i;
    if (!session_id || !cred_ref || !buf || buflen == 0)
        return CREDSCOPE_E_INVAL;
    for (i = 0; i < CREDSCOPE_MAX_LEASES; i++) {
        credscope_lease *l = &g_credscope.leases[i];
        if (l->used &&
            strcmp(l->session_id, session_id) == 0 &&
            strcmp(l->cred_ref, cred_ref) == 0) {
            int w = snprintf(buf, buflen,
                             "scope[%s/%s] flags=%s(0x%02x) expiry=%llu",
                             l->session_id, l->cred_ref,
                             credscope_flags_name(l->flags),
                             (unsigned)l->flags,
                             (unsigned long long)l->expiry_ms);
            if (w < 0 || (size_t)w >= buflen)
                return CREDSCOPE_E_INVAL;
            return w;
        }
    }
    if (buflen > 0) buf[0] = '\0';
    return CREDSCOPE_E_NOENT;
}

int credscope_count(void)
{
    return g_credscope.count;
}

const char *credscope_strerror(int err)
{
    switch (err) {
        case CREDSCOPE_OK:       return "ok";
        case CREDSCOPE_E_INVAL:  return "invalid argument";
        case CREDSCOPE_E_FULL:   return "lease table full";
        case CREDSCOPE_E_NOENT:  return "no such lease";
        case CREDSCOPE_E_DENIED: return "access denied";
        default:                 return "unknown error";
    }
}
