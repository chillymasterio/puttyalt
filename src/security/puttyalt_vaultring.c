/* puttyalt_vaultring.c - In-memory keyring abstraction over a secrets vault.
 * Named slots hold XOR-obfuscated secret blobs to avoid plaintext residency.
 * All symbols prefixed vaultring_. C99, self-contained.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VAULTRING_MAX_SLOTS   32
#define VAULTRING_NAME_LEN    48
#define VAULTRING_SECRET_LEN  256
#define VAULTRING_MASK        0x5Au

enum {
    VAULTRING_OK = 0,
    VAULTRING_ERR_LOCKED = 1,
    VAULTRING_ERR_FULL = 2,
    VAULTRING_ERR_NOTFOUND = 3,
    VAULTRING_ERR_INVAL = 4
};

typedef struct vaultring_slot {
    int      used;
    char     name[VAULTRING_NAME_LEN];
    uint8_t  secret[VAULTRING_SECRET_LEN]; /* XOR-masked */
    size_t   secret_len;
    uint64_t added_ms;
} vaultring_slot;

typedef struct vaultring {
    vaultring_slot slots[VAULTRING_MAX_SLOTS];
    int      locked;
    uint32_t check;        /* passphrase-derived check value */
    int      last_error;
} vaultring;

static uint32_t vaultring_derive(const char *pass)
{
    uint32_t h = 2166136261u;
    if (pass) {
        for (; *pass; pass++) { h ^= (uint8_t)*pass; h *= 16777619u; }
    }
    return h ? h : 1u;
}

void vaultring_init(vaultring *v)
{
    if (!v) return;
    memset(v, 0, sizeof(*v));
    v->locked = 1;
    v->last_error = VAULTRING_OK;
}

int vaultring_unlock(vaultring *v, const char *passphrase)
{
    uint32_t d;
    if (!v || !passphrase) return -1;
    d = vaultring_derive(passphrase);
    if (v->check == 0u) v->check = d;        /* first unlock sets check */
    if (v->check != d) { v->last_error = VAULTRING_ERR_INVAL; return -1; }
    v->locked = 0;
    v->last_error = VAULTRING_OK;
    return 0;
}

int vaultring_lock(vaultring *v)
{
    if (!v) return -1;
    v->locked = 1;
    v->last_error = VAULTRING_OK;
    return 0;
}

int vaultring_is_locked(const vaultring *v)
{
    return (!v || v->locked) ? 1 : 0;
}

int vaultring_put(vaultring *v, const char *name,
                  const uint8_t *secret, size_t len, uint64_t now_ms)
{
    int free_idx = -1;
    size_t i;
    if (!v || !name || !name[0] || !secret) return -1;
    if (v->locked) { v->last_error = VAULTRING_ERR_LOCKED; return -1; }
    if (len > VAULTRING_SECRET_LEN || strlen(name) >= VAULTRING_NAME_LEN) {
        v->last_error = VAULTRING_ERR_INVAL; return -1;
    }
    for (i = 0; i < VAULTRING_MAX_SLOTS; i++) {
        if (v->slots[i].used && strcmp(v->slots[i].name, name) == 0) {
            free_idx = (int)i; break;
        }
        if (free_idx < 0 && !v->slots[i].used) free_idx = (int)i;
    }
    if (free_idx < 0) { v->last_error = VAULTRING_ERR_FULL; return -1; }
    {
        vaultring_slot *s = &v->slots[free_idx];
        memset(s, 0, sizeof(*s));
        s->used = 1;
        strncpy(s->name, name, VAULTRING_NAME_LEN - 1);
        for (i = 0; i < len; i++) s->secret[i] = secret[i] ^ VAULTRING_MASK;
        s->secret_len = len;
        s->added_ms = now_ms;
    }
    v->last_error = VAULTRING_OK;
    return 0;
}

int vaultring_get(vaultring *v, const char *name,
                  uint8_t *out, size_t out_cap, size_t *out_len)
{
    size_t i, j;
    if (!v || !name || !out) return -1;
    if (v->locked) { v->last_error = VAULTRING_ERR_LOCKED; return -1; }
    for (i = 0; i < VAULTRING_MAX_SLOTS; i++) {
        vaultring_slot *s = &v->slots[i];
        if (!s->used || strcmp(s->name, name) != 0) continue;
        if (s->secret_len > out_cap) { v->last_error = VAULTRING_ERR_INVAL; return -1; }
        for (j = 0; j < s->secret_len; j++) out[j] = s->secret[j] ^ VAULTRING_MASK;
        if (out_len) *out_len = s->secret_len;
        v->last_error = VAULTRING_OK;
        return 0;
    }
    v->last_error = VAULTRING_ERR_NOTFOUND;
    return -1;
}

int vaultring_remove(vaultring *v, const char *name)
{
    size_t i;
    if (!v || !name) return -1;
    if (v->locked) { v->last_error = VAULTRING_ERR_LOCKED; return -1; }
    for (i = 0; i < VAULTRING_MAX_SLOTS; i++) {
        if (v->slots[i].used && strcmp(v->slots[i].name, name) == 0) {
            memset(&v->slots[i], 0, sizeof(v->slots[i]));
            v->last_error = VAULTRING_OK;
            return 0;
        }
    }
    v->last_error = VAULTRING_ERR_NOTFOUND;
    return -1;
}

int vaultring_list(const vaultring *v, char *buf, size_t cap)
{
    size_t i, off = 0;
    int n = 0;
    if (!v || !buf || cap == 0) return -1;
    buf[0] = '\0';
    for (i = 0; i < VAULTRING_MAX_SLOTS; i++) {
        int w;
        if (!v->slots[i].used) continue;
        w = snprintf(buf + off, cap - off, "%s%s (%lu bytes)",
                     n ? "\n" : "", v->slots[i].name,
                     (unsigned long)v->slots[i].secret_len);
        if (w < 0 || (size_t)w >= cap - off) { buf[off] = '\0'; break; }
        off += (size_t)w;
        n++;
    }
    return n;
}

int vaultring_count(const vaultring *v)
{
    int n = 0;
    size_t i;
    if (!v) return -1;
    for (i = 0; i < VAULTRING_MAX_SLOTS; i++)
        if (v->slots[i].used) n++;
    return n;
}

int vaultring_clear(vaultring *v)
{
    if (!v) return -1;
    memset(v->slots, 0, sizeof(v->slots));
    v->locked = 1;
    v->last_error = VAULTRING_OK;
    return 0;
}

const char *vaultring_strerror(int err)
{
    switch (err) {
        case VAULTRING_OK:           return "ok";
        case VAULTRING_ERR_LOCKED:   return "vault is locked";
        case VAULTRING_ERR_FULL:     return "vault is full";
        case VAULTRING_ERR_NOTFOUND: return "slot not found";
        case VAULTRING_ERR_INVAL:    return "invalid argument";
        default:                     return "unknown error";
    }
}
