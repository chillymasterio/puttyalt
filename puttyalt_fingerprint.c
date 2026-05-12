#include "puttyalt_fingerprint.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void fpstore_init(FPStore *fps)
{
    memset(fps, 0, sizeof(*fps));
}

int fpstore_add(FPStore *fps, const char *host, int port,
                const char *fingerprint, const char *algo)
{
    if (fps->count >= FP_MAX_HOSTS) return -1;
    int idx = fpstore_find(fps, host, port);
    if (idx >= 0) return idx;

    FPEntry *e = &fps->entries[fps->count];
    memset(e, 0, sizeof(*e));
    snprintf(e->hostname, FP_MAX_HOST, "%s", host);
    e->port = port;
    snprintf(e->fingerprint, FP_MAX_FP, "%s", fingerprint);
    snprintf(e->algo, FP_MAX_ALGO, "%s", algo);
    e->trust = FP_TRUST_UNKNOWN;
    e->first_seen = (long)time(NULL);
    e->last_seen = e->first_seen;
    e->seen_count = 1;
    return fps->count++;
}

int fpstore_find(const FPStore *fps, const char *host, int port)
{
    for (int i = 0; i < fps->count; i++) {
        if (strcmp(fps->entries[i].hostname, host) == 0 &&
            fps->entries[i].port == port)
            return i;
    }
    return -1;
}

FPTrustLevel fpstore_verify(FPStore *fps, const char *host, int port,
                            const char *fingerprint)
{
    int idx = fpstore_find(fps, host, port);
    if (idx < 0) return FP_TRUST_UNKNOWN;

    FPEntry *e = &fps->entries[idx];
    e->last_seen = (long)time(NULL);
    e->seen_count++;

    if (e->trust == FP_TRUST_REVOKED) return FP_TRUST_REVOKED;
    if (strcmp(e->fingerprint, fingerprint) != 0) {
        e->trust = FP_TRUST_CHANGED;
        return FP_TRUST_CHANGED;
    }
    return e->trust;
}

int fpstore_trust(FPStore *fps, int index)
{
    if (index < 0 || index >= fps->count) return -1;
    fps->entries[index].trust = FP_TRUST_TRUSTED;
    return 0;
}

int fpstore_revoke(FPStore *fps, int index)
{
    if (index < 0 || index >= fps->count) return -1;
    fps->entries[index].trust = FP_TRUST_REVOKED;
    return 0;
}

int fpstore_remove(FPStore *fps, int index)
{
    if (index < 0 || index >= fps->count) return -1;
    for (int i = index; i < fps->count - 1; i++)
        fps->entries[i] = fps->entries[i + 1];
    fps->count--;
    return 0;
}

int fpstore_load(FPStore *fps, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return -1;
    fpstore_init(fps);
    FPEntry *cur = NULL;

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[host]") == 0) {
            if (fps->count >= FP_MAX_HOSTS) break;
            cur = &fps->entries[fps->count++];
            memset(cur, 0, sizeof(*cur));
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->hostname, FP_MAX_HOST, "%s", line + 5);
        else if (strncmp(line, "port=", 5) == 0)
            cur->port = atoi(line + 5);
        else if (strncmp(line, "fp=", 3) == 0)
            snprintf(cur->fingerprint, FP_MAX_FP, "%s", line + 3);
        else if (strncmp(line, "algo=", 5) == 0)
            snprintf(cur->algo, FP_MAX_ALGO, "%s", line + 5);
        else if (strncmp(line, "trust=", 6) == 0)
            cur->trust = atoi(line + 6);
    }
    fclose(f);
    return 0;
}

int fpstore_save(const FPStore *fps, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < fps->count; i++) {
        const FPEntry *e = &fps->entries[i];
        fprintf(f, "[host]\nname=%s\nport=%d\nfp=%s\nalgo=%s\ntrust=%d\n\n",
                e->hostname, e->port, e->fingerprint, e->algo, e->trust);
    }
    fclose(f);
    return 0;
}

int fpstore_export_known_hosts(const FPStore *fps, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < fps->count; i++) {
        const FPEntry *e = &fps->entries[i];
        if (e->trust == FP_TRUST_TRUSTED) {
            if (e->port == 22)
                fprintf(f, "%s %s %s\n", e->hostname, e->algo, e->fingerprint);
            else
                fprintf(f, "[%s]:%d %s %s\n", e->hostname, e->port,
                        e->algo, e->fingerprint);
        }
    }
    fclose(f);
    return 0;
}
