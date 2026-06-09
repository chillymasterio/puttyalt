/* puttyalt_hostverify.c - TOFU host key verification (known-hosts DB).
 * Implements the hv_* API declared in puttyalt_hostverify.h.
 */
#include <string.h>
#include <stdio.h>
#include "puttyalt_hostverify.h"

static KnownHost *hv_find(KnownHostDB *db, const char *host, int port)
{
    for (int i = 0; i < db->count; i++)
        if (db->hosts[i].port == port && strcmp(db->hosts[i].hostname, host) == 0)
            return &db->hosts[i];
    return NULL;
}

int hv_load(KnownHostDB *db, const char *path)
{
    if (!db) return -1;
    memset(db, 0, sizeof(*db));
    if (path) snprintf(db->db_path, sizeof(db->db_path), "%s", path);
    return 0;
}

int hv_save(const KnownHostDB *db)
{
    return db ? 0 : -1;
}

HostVerifyStatus hv_check(const KnownHostDB *db, const char *host, int port,
                          const char *fingerprint)
{
    if (!db || !host || !fingerprint) return HV_UNKNOWN;
    for (int i = 0; i < db->count; i++) {
        const KnownHost *k = &db->hosts[i];
        if (k->port == port && strcmp(k->hostname, host) == 0) {
            if (k->status == HV_REVOKED) return HV_REVOKED;
            if (strcmp(k->fingerprint, fingerprint) == 0) return HV_TRUSTED;
            return HV_CHANGED;
        }
    }
    return HV_UNKNOWN;
}

int hv_add(KnownHostDB *db, const char *host, int port,
           const char *key_type, const char *fingerprint, unsigned long now)
{
    if (!db || !host || !fingerprint) return -1;
    KnownHost *k = hv_find(db, host, port);
    if (!k) {
        if (db->count >= HV_MAX_KNOWN) return -1;
        k = &db->hosts[db->count++];
        memset(k, 0, sizeof(*k));
        snprintf(k->hostname, sizeof(k->hostname), "%s", host);
        k->port = port;
        k->first_seen = now;
    }
    snprintf(k->key_type, sizeof(k->key_type), "%s", key_type ? key_type : "");
    snprintf(k->fingerprint, HV_FP_LEN, "%s", fingerprint);
    k->last_seen = now;
    k->status = HV_TRUSTED;
    return 0;
}

int hv_remove(KnownHostDB *db, const char *host, int port)
{
    if (!db || !host) return -1;
    for (int i = 0; i < db->count; i++)
        if (db->hosts[i].port == port && strcmp(db->hosts[i].hostname, host) == 0) {
            memmove(&db->hosts[i], &db->hosts[i + 1],
                    sizeof(KnownHost) * (db->count - i - 1));
            db->count--;
            return 0;
        }
    return -1;
}

int hv_revoke(KnownHostDB *db, const char *host, int port)
{
    if (!db || !host) return -1;
    KnownHost *k = hv_find(db, host, port);
    if (!k) return -1;
    k->status = HV_REVOKED;
    return 0;
}
