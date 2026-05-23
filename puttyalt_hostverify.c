#include <string.h>
#include <stdio.h>
#include "puttyalt_hostverify.h"

int hv_load(KnownHostDB *db, const char *path)
{
    memset(db, 0, sizeof(*db));
    strncpy(db->db_path, path, sizeof(db->db_path) - 1);

    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    char line[1024];
    while (fgets(line, sizeof(line), fp) && db->count < HV_MAX_KNOWN) {
        KnownHost *h = &db->hosts[db->count];
        if (sscanf(line, "%255s %d %31s %127s %lu",
                   h->hostname, &h->port, h->key_type,
                   h->fingerprint, &h->first_seen) >= 4) {
            h->status = HV_TRUSTED;
            h->last_seen = h->first_seen;
            db->count++;
        }
    }
    fclose(fp);
    return db->count;
}

int hv_save(const KnownHostDB *db)
{
    FILE *fp = fopen(db->db_path, "w");
    if (!fp) return -1;
    for (int i = 0; i < db->count; i++) {
        const KnownHost *h = &db->hosts[i];
        if (h->status == HV_REVOKED) continue;
        fprintf(fp, "%s %d %s %s %lu\n",
                h->hostname, h->port, h->key_type,
                h->fingerprint, h->first_seen);
    }
    fclose(fp);
    return 0;
}

HostVerifyStatus hv_check(const KnownHostDB *db, const char *host, int port,
                          const char *fingerprint)
{
    for (int i = 0; i < db->count; i++) {
        const KnownHost *h = &db->hosts[i];
        if (strcmp(h->hostname, host) == 0 && h->port == port) {
            if (h->status == HV_REVOKED) return HV_REVOKED;
            if (strcmp(h->fingerprint, fingerprint) == 0)
                return HV_TRUSTED;
            return HV_CHANGED; /* key mismatch! */
        }
    }
    return HV_UNKNOWN;
}

int hv_add(KnownHostDB *db, const char *host, int port,
           const char *key_type, const char *fingerprint, unsigned long now)
{
    if (!db || !host || !key_type || !fingerprint) return -1;
    if (db->count >= HV_MAX_KNOWN) return -1;
    KnownHost *h = &db->hosts[db->count];
    memset(h, 0, sizeof(*h));
    snprintf(h->hostname, sizeof(h->hostname), "%s", host);
    h->port = port;
    snprintf(h->key_type, sizeof(h->key_type), "%s", key_type);
    snprintf(h->fingerprint, HV_FP_LEN, "%s", fingerprint);
    h->first_seen = now;
    h->last_seen = now;
    h->status = HV_TRUSTED;
    db->count++;
    return 0;
}

int hv_remove(KnownHostDB *db, const char *host, int port)
{
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->hosts[i].hostname, host) == 0 && db->hosts[i].port == port) {
            for (int j = i; j < db->count - 1; j++)
                db->hosts[j] = db->hosts[j + 1];
            db->count--;
            return 0;
        }
    }
    return -1;
}

int hv_revoke(KnownHostDB *db, const char *host, int port)
{
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->hosts[i].hostname, host) == 0 && db->hosts[i].port == port) {
            db->hosts[i].status = HV_REVOKED;
            return 0;
        }
    }
    return -1;
}
