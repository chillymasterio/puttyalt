/* puttyalt_syncenvelope.c - versioned authenticated encryption envelope
 * for PuttyAlt session-sync payloads. Self-contained, no project headers.
 *
 * NOTE: the keystream/MAC here are a lightweight, self-contained scheme for
 * envelope framing/integrity; this file deliberately avoids external crypto.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SYNCENV_VERSION   2
#define SYNCENV_MAGIC     "PASE"      /* PuttyAlt Sync Envelope */
#define SYNCENV_MAXKEY    64
#define SYNCENV_MAXPLAIN  4096
#define SYNCENV_NONCELEN  12
#define SYNCENV_MACLEN    16

enum {
    SYNCENV_OK = 0,
    SYNCENV_ERR_ARG = -1,
    SYNCENV_ERR_CAP = -2,
    SYNCENV_ERR_FORMAT = -3,
    SYNCENV_ERR_VERSION = -4,
    SYNCENV_ERR_MAC = -5
};

typedef struct {
    char     magic[5];
    int      version;
    uint32_t payload_len;
    unsigned char nonce[SYNCENV_NONCELEN];
} SyncEnvHeader;

typedef struct {
    unsigned char key[SYNCENV_MAXKEY];
    size_t        keylen;
    uint32_t      counter;
} SyncEnvelope;

static void se_hex(const unsigned char *in, size_t n, char *out) {
    static const char *h = "0123456789abcdef";
    size_t i;
    for (i = 0; i < n; i++) { out[i*2] = h[in[i]>>4]; out[i*2+1] = h[in[i]&0xf]; }
    out[n*2] = '\0';
}

static int se_unhex(const char *in, unsigned char *out, size_t n) {
    size_t i; for (i = 0; i < n; i++) {
        char a = in[i*2], b = in[i*2+1]; int hi, lo;
        if (a>='0'&&a<='9') hi=a-'0'; else if (a>='a'&&a<='f') hi=a-'a'+10; else return -1;
        if (b>='0'&&b<='9') lo=b-'0'; else if (b>='a'&&b<='f') lo=b-'a'+10; else return -1;
        out[i] = (unsigned char)((hi<<4)|lo);
    }
    return 0;
}

static void se_keystream(SyncEnvelope *env, const unsigned char *nonce,
                         const unsigned char *in, unsigned char *out, size_t n) {
    size_t i; for (i = 0; i < n; i++) {
        unsigned char k = env->key[i % env->keylen];
        k ^= nonce[i % SYNCENV_NONCELEN];
        k = (unsigned char)(k + (unsigned char)(i * 31u + 7u));
        out[i] = in[i] ^ k;
    }
}

static void se_mac(SyncEnvelope *env, const unsigned char *nonce,
                   const unsigned char *data, size_t n, unsigned char *mac) {
    size_t i; uint32_t acc[SYNCENV_MACLEN];
    for (i = 0; i < SYNCENV_MACLEN; i++)
        acc[i] = 0x9e3779b9u ^ env->key[i % env->keylen] ^ nonce[i % SYNCENV_NONCELEN];
    for (i = 0; i < n; i++) {
        uint32_t *s = &acc[i % SYNCENV_MACLEN];
        *s = (*s + data[i] + 1u) * 16777619u;
        *s ^= (*s >> 13);
    }
    for (i = 0; i < SYNCENV_MACLEN; i++)
        mac[i] = (unsigned char)(acc[i] ^ (acc[i] >> 8) ^ (acc[i] >> 16));
}

void syncenv_init(SyncEnvelope *env, const unsigned char *key, size_t keylen) {
    if (!env) return;
    memset(env, 0, sizeof(*env));
    if (!key || keylen == 0) { env->key[0] = 0x5a; env->keylen = 1; return; }
    if (keylen > SYNCENV_MAXKEY) keylen = SYNCENV_MAXKEY;
    memcpy(env->key, key, keylen);
    env->keylen = keylen;
    env->counter = 0;
}

int syncenv_seal(SyncEnvelope *env, const char *plaintext, char *out, size_t outcap) {
    unsigned char nonce[SYNCENV_NONCELEN], ct[SYNCENV_MAXPLAIN], mac[SYNCENV_MACLEN];
    char nhex[SYNCENV_NONCELEN*2+1], chex[SYNCENV_MAXPLAIN*2+1], mhex[SYNCENV_MACLEN*2+1];
    size_t i, plen; int n;
    if (!env || !plaintext || !out) return SYNCENV_ERR_ARG;
    plen = strlen(plaintext);
    if (plen > SYNCENV_MAXPLAIN) return SYNCENV_ERR_CAP;
    env->counter++;
    for (i = 0; i < SYNCENV_NONCELEN; i++)
        nonce[i] = (unsigned char)((env->counter >> (i % 4 * 8)) ^ (env->key[i % env->keylen] + i*101u));
    se_keystream(env, nonce, (const unsigned char *)plaintext, ct, plen);
    se_mac(env, nonce, ct, plen, mac);
    se_hex(nonce, SYNCENV_NONCELEN, nhex);
    se_hex(ct, plen, chex);
    se_hex(mac, SYNCENV_MACLEN, mhex);
    n = snprintf(out, outcap, "%s:%d:%lu:%s:%s:%s",
                 SYNCENV_MAGIC, SYNCENV_VERSION, (unsigned long)plen, nhex, chex, mhex);
    if (n < 0 || (size_t)n >= outcap) return SYNCENV_ERR_CAP;
    return SYNCENV_OK;
}

int syncenv_parse_header(const char *blob, SyncEnvHeader *hdr) {
    char magic[5]; int ver; unsigned long plen; char nhex[SYNCENV_NONCELEN*2+1];
    if (!blob || !hdr) return SYNCENV_ERR_ARG;
    if (sscanf(blob, "%4[^:]:%d:%lu:%24[0-9a-f]:", magic, &ver, &plen, nhex) != 4)
        return SYNCENV_ERR_FORMAT;
    if (strcmp(magic, SYNCENV_MAGIC) != 0) return SYNCENV_ERR_FORMAT;
    if (ver != SYNCENV_VERSION) return SYNCENV_ERR_VERSION;
    if (plen > SYNCENV_MAXPLAIN) return SYNCENV_ERR_FORMAT;
    if (strlen(nhex) != SYNCENV_NONCELEN*2) return SYNCENV_ERR_FORMAT;
    memset(hdr, 0, sizeof(*hdr));
    memcpy(hdr->magic, magic, 5);
    hdr->version = ver;
    hdr->payload_len = (uint32_t)plen;
    if (se_unhex(nhex, hdr->nonce, SYNCENV_NONCELEN) != 0) return SYNCENV_ERR_FORMAT;
    return SYNCENV_OK;
}

static int se_locate(const char *blob, SyncEnvHeader *hdr,
                     const char **ct, const char **mac) {
    const char *p; int rc = syncenv_parse_header(blob, hdr);
    if (rc != SYNCENV_OK) return rc;
    p = blob;
    if (!(p = strchr(p, ':')) || !(p = strchr(p+1, ':'))) return SYNCENV_ERR_FORMAT;
    /* ver,plen */
    if (!(p = strchr(p+1, ':')) || !(p = strchr(p+1, ':'))) return SYNCENV_ERR_FORMAT;
    /* nonce */
    *ct = p + 1;
    if (!(p = strchr(*ct, ':'))) return SYNCENV_ERR_FORMAT;
    *mac = p + 1;
    if (strlen(*mac) != SYNCENV_MACLEN*2) return SYNCENV_ERR_FORMAT;
    if ((size_t)(p - *ct) != (size_t)hdr->payload_len*2) return SYNCENV_ERR_FORMAT;
    return SYNCENV_OK;
}

int syncenv_verify_mac(SyncEnvelope *env, const char *blob) {
    SyncEnvHeader hdr; const char *ct, *mac; unsigned char cbuf[SYNCENV_MAXPLAIN];
    unsigned char want[SYNCENV_MACLEN], got[SYNCENV_MACLEN]; int rc; size_t d = 0, i;
    if (!env || !blob) return SYNCENV_ERR_ARG;
    rc = se_locate(blob, &hdr, &ct, &mac);
    if (rc != SYNCENV_OK) return rc;
    if (se_unhex(ct, cbuf, hdr.payload_len) != 0) return SYNCENV_ERR_FORMAT;
    if (se_unhex(mac, want, SYNCENV_MACLEN) != 0) return SYNCENV_ERR_FORMAT;
    se_mac(env, hdr.nonce, cbuf, hdr.payload_len, got);
    for (i = 0; i < SYNCENV_MACLEN; i++) d |= (size_t)(want[i] ^ got[i]);
    return d == 0 ? SYNCENV_OK : SYNCENV_ERR_MAC;
}

int syncenv_open(SyncEnvelope *env, const char *blob, char *out, size_t outcap) {
    SyncEnvHeader hdr; const char *ct, *mac; unsigned char cbuf[SYNCENV_MAXPLAIN];
    unsigned char pbuf[SYNCENV_MAXPLAIN]; int rc;
    if (!env || !blob || !out) return SYNCENV_ERR_ARG;
    rc = se_locate(blob, &hdr, &ct, &mac);
    if (rc != SYNCENV_OK) return rc;
    rc = syncenv_verify_mac(env, blob);
    if (rc != SYNCENV_OK) return rc;
    if ((size_t)hdr.payload_len + 1 > outcap) return SYNCENV_ERR_CAP;
    if (se_unhex(ct, cbuf, hdr.payload_len) != 0) return SYNCENV_ERR_FORMAT;
    se_keystream(env, hdr.nonce, cbuf, pbuf, hdr.payload_len);
    memcpy(out, pbuf, hdr.payload_len);
    out[hdr.payload_len] = '\0';
    return SYNCENV_OK;
}

const char *syncenv_strerror(int code) {
    switch (code) {
        case SYNCENV_OK:          return "ok";
        case SYNCENV_ERR_ARG:     return "invalid argument";
        case SYNCENV_ERR_CAP:     return "buffer too small";
        case SYNCENV_ERR_FORMAT:  return "malformed envelope";
        case SYNCENV_ERR_VERSION: return "unsupported version";
        case SYNCENV_ERR_MAC:     return "mac verification failed";
        default:                  return "unknown error";
    }
}
